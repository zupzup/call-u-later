#include "common.h"
#include <stdlib.h>

void remove_spaces(char *str) {
    char *temp = str;
    char *tmp = str;
    while (*temp != '\0') {
        *tmp = *temp++;
        if (!isspace(*tmp)) tmp++;
    }
    *tmp = '\0';
}

char *method_to_text(enum METHOD m) {
    if (m == GET) return "GET";
    if (m == POST) return "POST";
    if (m == PUT) return "PUT";
    if (m == DELETE) return "DELETE";
    if (m == OPTIONS) return "OPTIONS";
    if (m == HEAD) return "HEAD";
    return NULL;
}
