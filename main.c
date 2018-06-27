#include <stdio.h>
#include "options.h"
#include "request.h"

#define BINARY_NAME "call-u-later"

void print_usage() {
    printf("Usage:\n\n%s [OPTIONS] url\n\nOptions:\n\nTBD\n", BINARY_NAME);
}

// TODO: https: http://fm4dd.com/openssl/sslconnect.htm
// TODO: use host for request
// TODO: create request dynamically (with headers)
// TODO dynamically allocate response, no matter the size
// TODO format output
// TODO POST with body in request
// TODO implement HTTPS

int main(int argv, char** argc) {
    if (argv <= 1) {
        print_usage();
        return TOO_FEW_ARGUMENTS;
    }

    Options opts = create_options(argv, argc);
    int err = parse_options(argv, argc, &opts);
    if (err != 0) {
        return err;
    }
    printf("Options:\n");
    printf("Method: %d\n", opts.method);
    printf("Headers (%d):\n", opts.num_headers);
    for (uint i = 0; i < opts.num_headers; i++) {
        printf("\t%s:%s\n", opts.headers[i]->key, opts.headers[i]->value);
    }
    printf("Host: %s\n", opts.host);
    printf("Path: %s\n", opts.path);
    printf("Body: %s\n", opts.body);
    int request_result = make_request(&opts);
    destroy_options(&opts);
    return request_result;
}
