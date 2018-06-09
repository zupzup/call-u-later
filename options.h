#ifndef OPTIONS_H
#define OPTIONS_H

#include "common.h"
#include <stdbool.h>

typedef struct Options {
    enum METHOD method;
    char *url;
    Header **headers;
    uint num_headers;
} Options;

int parse_options(int argv, char **argc, Options *opts);
Options create_options(int argv);
void destroy_options(Options *opts);
bool is_flag(char *arg);

#endif
