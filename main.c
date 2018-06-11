#include <stdio.h>
#include "options.h"

#define BINARY_NAME ("call-u-later")
#define HTTP_VERSION ("HTTP/1.1")

void print_usage() {
    printf("Usage:\n\n%s [OPTIONS] url\n\nOptions:\n\nTBD\n", BINARY_NAME);
}

char *create_http_payload(Options *opts) {
    if (!opts) return "";
    printf("Request:\n\n");
    printf("%s %s %s\r\n", method_to_text(opts->method), opts->path, HTTP_VERSION);
    printf("Host: %s\r\n", opts->host);
    return "";
}

int make_request(Options *opts) {
    printf("%s\n", create_http_payload(opts));
    return 0;
}

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
