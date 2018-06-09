#ifndef COMMON_H
#define COMMON_H

#define HYPHEN ('-')

#include <ctype.h>

typedef unsigned int uint;

typedef struct Header {
    char *key;
    char *value;
} Header;

enum METHOD {
    GET,
    POST,
    PUT,
    DELETE,
    OPTIONS,
    HEAD,
    METHOD_SIZE
};

enum ERROR_CODES {
    INVALID_FLAG = 1,
    TOO_FEW_ARGUMENTS,
};

enum FLAG {
    METHOD,
    HEADER,
    UNKNOWN,
    FLAG_SIZE
};

typedef struct Flag {
    enum FLAG flag;
    char *value;
} Flag;

void remove_spaces(char *str);

#endif
