
#include "server.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "file.h"

int run_server(int var1) {
    int server_fd, client_fd;
    //TODO Process ID, child processes
    pid_t pid;
    //What is sent to the client
    char string_buffer[MAX_LENGTH];
    socklen_t client_address_length;
    struct sockaddr_in server_address, client_address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        fprintf(stderr, "Socket failed with code %d: %s\n", errno, strerror(errno));
        return 1;
    }

    //Memset to clear the junk values stored in the memory address
    memset(&server_address, 0, sizeof(server_address));
    if (get_file_contents(FIRST_FILE, string_buffer) != 0) {
        fprintf(stderr, "Failed to read file contents. Aborting...\n");
        return 1;
    }

    //**Set the server information**
    //AF_INET means it's using IPv4
    server_address.sin_family = AF_INET;
    //Allow all incoming addresses
    server_address.sin_addr.s_addr = INADDR_ANY;
    //Convert port to big endian ordering (most significant value first)
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "Bind failed with code %d: %s\n", errno, strerror(errno));
        close(server_fd);
        return 1;
    }
    //True
    int option = 1;
    //Allow reuse of local addresses, causes errors if not set to true
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
    //Start listening for anyone attempting to connect
    listen(server_fd, MAX_CONNECTIONS);

    printf("%s\n", "Server running...waiting for connections.");

    while(1) {
        client_address_length = sizeof(client_address);
        if ((client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_address_length)) < 0) {
            fprintf(stderr, "Accept failed with code %d: %s\n", errno, strerror(errno));
            return 1;
        }
        char *ip = inet_ntoa(client_address.sin_addr);
        printf("%s from ip %s\n", "Received request...", ip);
        // pid = fork(); TODO

        printf("sending \"%s\"\n", string_buffer);
        send(client_fd, &string_buffer, strlen(string_buffer) * sizeof(char), 0);
        close(client_fd);

    }
    //close listening socket
    close(server_fd);

    return 0;
}
