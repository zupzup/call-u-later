#include "common.h"

void remove_spaces(char *str) {
    char *temp = str;
    char *tmp = str;
    while (*temp != '\0') {
        *tmp = *temp++;
        if (!isspace(*tmp)) tmp++;
    }
    *tmp = '\0';
}
