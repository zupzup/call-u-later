#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

enum ERROR_CODES {
    INVALID_FLAG = 1,
    TOO_FEW_ARGUMENTS,
};

enum METHOD {
    GET,
    POST,
    PUT,
    DELETE,
    OPTIONS,
    HEAD
};

enum FLAG {
    METHOD,
    HEADER,
    UNKNOWN
};

typedef struct flag {
    enum FLAG flag;
    char *value;
} flag;

typedef struct options {
    enum METHOD method;
} options;

static const char BINARY_NAME[] = "call-u-later";
static const char HYPHEN = '-';

void print_usage() {
    printf("Usage:\n\n%s [OPTIONS] url\n\nOptions:\n\nTBD\n", BINARY_NAME);
}

bool is_flag(char *arg) {
    if (arg == NULL || strlen(arg) <= 1) return false;
    return arg[0] == HYPHEN && ((arg[1] == HYPHEN && isalpha(arg[2])) || isalpha(arg[1])); 
}

enum FLAG str_to_flag(char *arg) {
    if (strcmp(arg, "-m") == 0 || strcmp(arg, "--m") == 0) return METHOD;
    if (strcmp(arg, "-h") == 0 || strcmp(arg, "--h") == 0) return HEADER;
    return UNKNOWN;
}

flag parse_flag(char *arg, char *value) {
    flag empty = { 0, 0 };
    if (is_flag(arg)) {
        flag res = { .flag = str_to_flag(arg), .value = value };
        return res;
    }
    return empty;
}

bool validate_flag(flag *f) {
    if (f->flag == METHOD && !f->value) {
        printf("error - you need to specify a request method after -m / --m\n");
        return false;
    }
    // TODO: validate request method
    if (f->flag == HEADER && !f->value) {
        printf("error - you need to specify a header after -h / --h\n");
        return false;
    }
    // TODO: validate header format `key: value`
    return true;
}

int parse_options(int argv, char **argc, options *Opts) {
    Opts->method = POST;
    // method for getting the next non -/-- thing after a flag
    // map methdos to METHOD
    // last param is the url
    // return error code (use enum)
    for (int i = 0; i < argv; i++) {
        if (is_flag(argc[i])) {
            char *value = NULL;
            if (i + 1 < argv && !is_flag(argc[i + 1])) {
                value = argc[i + 1];
            }
            flag f = parse_flag(argc[i], value);
            if (!validate_flag(&f)) return INVALID_FLAG; 
            printf("%s %s %i %s\n", argc[i], is_flag(argc[i]) ? "flag" : "value", f.flag, f.value);
        }
    }
    return 0;
}

int make_request(options *Opts) {
    printf("%d\n", Opts->method);
    return 0;
}

int main(int argv, char** argc) {
    if (argv <= 1) {
        print_usage();
        return TOO_FEW_ARGUMENTS;
    }

    options Opts = { 0 }; // TODO: default options struct
    int err = parse_options(argv, argc, &Opts);
    if (err != 0) {
        return err;
    }
    return make_request(&Opts);
}
