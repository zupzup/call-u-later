#ifndef COMMON_H
#define COMMON_H

#define HYPHEN ('-')

#include <ctype.h>

typedef unsigned int uint;

enum ERROR_CODES {
    INVALID_FLAG = 1,
    TOO_FEW_ARGUMENTS,
    NO_URL_PROVIDED,
    INVALID_URL,
    INVALID_METHOD,
};

enum METHOD {
    GET,
    POST,
    PUT,
    DELETE,
    OPTIONS,
    HEAD,
    METHOD_SIZE
};

void remove_spaces(char *str);
char *method_to_text(enum METHOD m);

#endif
