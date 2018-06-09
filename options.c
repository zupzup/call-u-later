#include "options.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

Options create_options(int argv) {
    Header **headers = (Header **)malloc((argv - 1) * sizeof(Header *));
    Options opts = { .method = GET, .url = "", .headers = headers, .num_headers = 0 };
    return opts;
}

bool is_flag(char *arg) {
    if (arg == NULL || strlen(arg) <= 1) return false;
    return arg[0] == HYPHEN && ((arg[1] == HYPHEN && isalpha(arg[2])) || isalpha(arg[1])); 
}

void destroy_options(Options *opts) {
    for (uint i = 0; i < opts->num_headers; i++) {
        free(opts->headers[i]->key);
        free(opts->headers[i]->value);
        free(opts->headers[i]);
    }
    free(opts->headers);
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
    return true;
}

enum FLAG str_to_flag(char *arg) {
    if (strcmp(arg, "-m") == 0 || strcmp(arg, "--m") == 0) return METHOD;
    if (strcmp(arg, "-h") == 0 || strcmp(arg, "--h") == 0) return HEADER;
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

int parse_options(int argv, char **argc, Options *opts) {
    opts->method = POST;
    opts->url = argc[argv - 1];
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
            printf("%s %s %i %s\n", argc[i], is_flag(argc[i]) ? "flag" : "value", f.flag, f.value);
        }
    }
    printf("method: %d url: %s num headers: %d key: %s, value %s\n", opts->method, opts->url, opts->num_headers, opts->headers[0]->key, opts->headers[0]->value);
    return 0;
}
