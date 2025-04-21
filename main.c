#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"
#include "file.h"

#define MAX_LENGTH 8192
#define SERVER_PORT 8080
#define MAX_CONNECTIONS 10

int main(int argc, char **argv) {
    int var = 0;
    char ch;
    while ((ch = getopt(argc, argv, "?ht:")) != -1) {
        switch (ch) {
            case 't':
                printf("%s\n", optarg);
                var = atoi(optarg);
                break;
            default: //Pass to next case
            case '?': //Pass to next case
            case 'h':
                printf("Usage: %s [-h] [-t num] [number]\n-t num: Which file to send to the client (0-2).\nDefault is 0.\n", argv[0]);
                return 0;
        }
    }

    return run_server(var);
}
