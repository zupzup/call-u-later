#include "options.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

Options create_options(int argv, char** argc) {
    Header **headers = (Header **)malloc((argv - 1) * sizeof(Header *));
    uint url_length = strlen(argc[argv - 1]);
    // TODO: parse URL here, for more efficient allocation
    char *host = (char *)calloc(url_length + 1, sizeof(char));
    char *path = (char *)calloc(url_length + 1, sizeof(char));
    char *port = (char *)calloc(6, sizeof(char));
    Options opts = { .method = GET, .host = host, .path = path, .body = "", .headers = headers, .num_headers = 0, .port = port, .is_tls = false };
    return opts;
}

void destroy_options(Options *opts) {
    for (uint i = 0; i < opts->num_headers; i++) {
        free(opts->headers[i]->key);
        free(opts->headers[i]->value);
        free(opts->headers[i]);
    }
    free(opts->host);
    free(opts->path);
    free(opts->headers);
}

bool is_flag(char *arg) {
    if (arg == NULL || strlen(arg) <= 1) return false;
    return arg[0] == HYPHEN && ((arg[1] == HYPHEN && isalpha(arg[2])) || isalpha(arg[1])); 
}

bool validate_flag(Flag *f, char *flag_as_string) {
    if (f->flag == UNKNOWN) {
        printf("error: %s is not a valid configuration option\n", flag_as_string);
        return false;
    }
    if (f->flag == METHOD && !f->value) {
        printf("error: you need to specify a request method after -m / --m\n");
        return false;
    }
    if (f->flag == HEADER && !f->value) {
        printf("error: you need to specify a header after -h / --h\n");
        return false;
    }
    if (f->flag == HEADER && strchr(f->value, ':') == NULL) {
        printf("error: a header needs to be of the format 'key:value', e.g.: 'Authorization: Basic 123' \n");
        return false;
    }
    if (f->flag == BODY && !f->value) {
        printf("error: you need to specify a body after -b / --b\n");
        return false;
    }
    return true;
}

enum METHOD str_to_method(char *m) {
    if (strcmp(m, "GET")) return GET;
    if (strcmp(m, "POST")) return GET;
    if (strcmp(m, "PUT")) return PUT;
    if (strcmp(m, "DELETE")) return DELETE;
    if (strcmp(m, "OPTIONS")) return OPTIONS;
    if (strcmp(m, "HEAD")) return HEAD;
    return -1;
}

enum FLAG str_to_flag(char *arg) {
    if (strcmp(arg, "-m") == 0 || strcmp(arg, "--m") == 0) return METHOD;
    if (strcmp(arg, "-h") == 0 || strcmp(arg, "--h") == 0) return HEADER;
    if (strcmp(arg, "-b") == 0 || strcmp(arg, "--b") == 0) return BODY;
    return UNKNOWN;
}

Flag parse_flag(char *arg, char *value) {
    Flag empty = { 0, 0 };
    if (is_flag(arg)) {
        Flag res = { .flag = str_to_flag(arg), .value = value };
        return res;
    }
    return empty;
}

Header *parse_header(Flag f) {
    Header *h = (Header *)malloc(sizeof(Header));
    char *v = f.value;
    uint len = strlen(v);
    uint colon_index = 0;
    for (uint i = 0; i < len; i++) {
        if (v[i] == ':') {
            colon_index = i;
            break;
        }
    }
    char *key = (char *)calloc(colon_index + 1, sizeof(char));
    memcpy(key, v, colon_index);
    char *value = (char *)calloc((len - colon_index), sizeof(char));
    memcpy(value, v + colon_index + 1, len - colon_index - 1);
    h->key = key;
    h->value = value;
    return h;
}

// TODO: more robust parsing
int parse_url(Options *opts, char *url) {
    if (!url) return NO_URL_PROVIDED;
    uint len = strlen(url);
    if (len < 8) return INVALID_URL;
    uint num_slashes = 0;
    uint num_colons = 0;
    uint path_index = 0;
    uint host_index = 0;
    uint port_index = 0;
    for (uint i = 0; i < len; i++) {
        if (url[i] == '/') num_slashes++;
        if (url[i] == ':') num_colons++; 
        if (num_colons == 2 && port_index == 0) port_index = i;
        if (num_slashes == 2 && url[i] == '/') host_index = i;
        if (num_slashes == 3) {
            path_index = i;
            break;
        }
    }
    if (num_slashes < 2) return INVALID_URL;
    if (host_index == 0) return INVALID_URL;
    if (num_slashes == 2) {
        if (port_index == 0) {
            memcpy(opts->host, url + host_index + 1, len - host_index - 1);
        } else {
            memcpy(opts->host, url + host_index + 1, port_index - host_index - 1);
            memcpy(opts->port, url + port_index + 1, len - port_index - 1);
        }
        memcpy(opts->path, "/", 1);
    } else {
        if (port_index == 0) {
            memcpy(opts->host, url + host_index + 1, path_index - host_index - 1);
        } else {
            memcpy(opts->host, url + host_index + 1, port_index - host_index - 1);
            memcpy(opts->port, url + port_index + 1, path_index - port_index - 1);
        }
        memcpy(opts->path, url + path_index, len - path_index);
    }
    if (strncmp("https", url, 5) == 0) {
        opts->is_tls = true;
    }
    return 0;
}

int parse_options(int argv, char **argc, Options *opts) {
    int err = parse_url(opts, argc[argv - 1]);
    if (err != 0 ) {
        return err;
    }
    for (int i = 0; i < argv; i++) {
        if (is_flag(argc[i])) {
            char *value = NULL;
            if (i + 1 < argv && !is_flag(argc[i + 1])) {
                value = argc[i + 1];
                remove_spaces(value);
            }
            Flag f = parse_flag(argc[i], value);
            if (!validate_flag(&f, argc[i])) return INVALID_FLAG; 
            if (f.flag == HEADER) {
                Header *h = parse_header(f);
                opts->headers[opts->num_headers++] = h;
            }
            if (f.flag == BODY) opts->body = f.value;
            if (f.flag == METHOD) {
                enum METHOD m = str_to_method(f.value);
                if (m >= GET && m < METHOD_SIZE) {
                    opts->method = m;
                } else {
                    return INVALID_METHOD;
                }
            }
        }
    }
    return 0;
}
