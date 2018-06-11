#ifndef OPTIONS_H
#define OPTIONS_H

#include "common.h"
#include <stdbool.h>

enum FLAG {
    METHOD,
    HEADER,
    BODY,
    UNKNOWN,
    FLAG_SIZE
};


typedef struct Flag {
    enum FLAG flag;
    char *value;
} Flag;

typedef struct Header {
    char *key;
    char *value;
} Header;

typedef struct Options {
    enum METHOD method;
    uint num_headers;
    Header **headers;
    char *host;
    char *path;
    char *body;
} Options;

int parse_options(int argv, char **argc, Options *opts);
Options create_options(int argv, char **argc);
void destroy_options(Options *opts);
bool is_flag(char *arg);

#endif
