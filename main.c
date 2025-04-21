#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"

int main(int argc, char **argv) {
    int var = 0;
    char ch;
    while ((ch = getopt(argc, argv, "?hf:")) != -1) {
        switch (ch) {
            case 'f':
                printf("%s\n", optarg);
                var = atoi(optarg);
                break;
            default: //Pass to next case
            case '?': //Pass to next case
            case 'h':
                printf("Usage: %s [-h] [-f num] [number]\n-f num: Which file to send to the client (0-2).\nDefault is 0.\n", argv[0]);
                return 0;
        }
    }

    return run_server(var);
}

/*Currently missing from ruberic:
    Bitfields with structs
    Environment variables
    malloc and free
    variadic function
 */
