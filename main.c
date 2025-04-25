#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#include "env.h"
#include "server.h"

int main(int argc, char **argv) {
    unsigned int port = DEFAULT_PORT;
    char ch;
    while ((ch = getopt(argc, argv, "?hp:")) != -1) {
        switch (ch) {
            case 'p':
                port = strtol(optarg, NULL, 0);
                if (port <= 0 || port > 65535) {
                    fprintf(stderr, "Invalid port number: %s. Using %d instead.\n", optarg, DEFAULT_PORT);
                    port = DEFAULT_PORT;
                }
                break;
            default: //Pass to h
            case '?': //Pass to h
            case 'h':
                printf("Usage: %s [-h] [-p port] [number]\n-p port: Which port to host the server on.\nDefault is 8080.\n", argv[0]);
                return 0;
        }
    }
    return run_server(port);
}