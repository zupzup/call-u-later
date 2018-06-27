#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define HTTP_VERSION "HTTP/1.1"
#define MAX_DATA_CHUNK 2048

char *create_http_payload(Options *opts) {
    if (!opts) return "";
    printf("Request:\n\n");
    printf("%s %s %s\r\n", method_to_text(opts->method), opts->path, HTTP_VERSION);
    printf("Host: %s\r\n", opts->host);
    printf("Port: %s\r\n", opts->port);
    printf("Path: %s\r\n", opts->path);
    printf("TLS: %d\r\n", opts->is_tls ? 1 : 0);
    return "";
}

void error(const char *msg) { perror(msg); exit(0); }

#define MAX_RESPONSE 1200000

int make_request(Options *opts) {
    printf("%s\n", create_http_payload(opts));
    /* first what are we going to send and where are we going to send it? */
    int portno = 80; 
    if (strlen(opts->port) > 0) portno = atoi(opts->port); 
    char *host = opts->host;

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024];

    char *response = (char *)calloc(MAX_RESPONSE, sizeof(char));

    /* fill in the parameters */
    sprintf(message, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
    printf("Request:\n%s\n", message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    total = MAX_RESPONSE - 1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);

    /* process response */
    printf("Response:\n%s\n",response);
    free(response);

    return 0;
}
