#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "env.h"
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
            default: //Pass to h
            case '?': //Pass to h
            case 'h':
                printf("Usage: %s [-h] [-f num] [number]\n-f num: Which file to send to the client (0-2). 0 is plain text, 1 is html, 2 is image.\nDefault is 0.\n", argv[0]);
                return 0;
        }
    }
    return run_server(var, argv[0]);
}

/*Currently missing from ruberic:
    Bitfields with structs
    malloc and free
    variadic function
 */
//opt 1 for text, 2 for html, 3 for image