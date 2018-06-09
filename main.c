#include <stdio.h>
#include "options.h"

#define BINARY_NAME ("call-u-later")

void print_usage() {
    printf("Usage:\n\n%s [OPTIONS] url\n\nOptions:\n\nTBD\n", BINARY_NAME);
}

int make_request(Options *opts) {
    printf("%d\n", opts->method);
    return 0;
}

int main(int argv, char** argc) {
    if (argv <= 1) {
        print_usage();
        return TOO_FEW_ARGUMENTS;
    }

    Options opts = create_options(argv);
    int err = parse_options(argv, argc, &opts);
    if (err != 0) {
        return err;
    }
    int request_result = make_request(&opts);
    destroy_options(&opts);
    return request_result;
}
