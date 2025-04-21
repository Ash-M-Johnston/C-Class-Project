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

    char ch;
    while ((ch = getopt(argc, argv, "?ht:")) != -1) {
        switch (ch) {
            case 't':
                printf("%s\n", optarg);
                break;
            default: //Pass to next case
            case '?': //Pass to next case
            case 'h':
                printf("Usage: %s [-h] [-t num] [number]\n-t num: Which file to send to the client (0-2).\nDefault is 0.\n", argv[0]);
                return 0;
        }
    }

    int server_fd, client_fd;
    //Process ID for concurrency
    pid_t pid;
    //What is sent to the client
    char string_buffer[MAX_LENGTH]; //*****3*******
    socklen_t client_address_len;
    struct sockaddr_in server_address, client_address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        fprintf(stderr, "Socket failed with code %d: %s\n", errno, strerror(errno));
        return errno;
    }

    //Memset to clear the junk values stored in the memory address
    memset(&server_address, 0, sizeof(server_address));
    get_file_contents(1, string_buffer);
    // strcat(string_buffer, "my very cool thing");

    //**Set the server information**
    //AF_INET means it's using IPv4
    server_address.sin_family = AF_INET;
    //Allow all incoming addresses
    server_address.sin_addr.s_addr = INADDR_ANY;
    //Convert port to big endian ordering (most significant value first)
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "Bind failed with code %d: %s\n", errno, strerror(errno));
        return errno;
    }
    int option=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
    //Start listening for anyone attempting to connect
    listen(server_fd, MAX_CONNECTIONS);


    printf("%s\n", "Server running...waiting for connections.");

    while(1) {
        client_address_len = sizeof(client_address);
        if ((client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_address_len)) < 0) {
            fprintf(stderr, "Accept failed with code %d: %s\n", errno, strerror(errno));
            return errno;
        }
        char *ip = inet_ntoa(client_address.sin_addr);
        printf("%s from ip %s\n", "Received request...", ip);
        // pid = fork(); concurrent, look into this

        printf("sending \"%s\"\n", string_buffer);
        send(client_fd, &string_buffer, strlen(string_buffer) * sizeof(char), 0);
        close(client_fd);

    }
    //close listening socket
    close(server_fd);

    return 0;
}
