
#include "server.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <regex.h>
#include <stdlib.h>
#include <pthread.h>

#include "file.h"
#include "env.h"

/*void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    // receive request data from client and store into buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        // check if request is GET
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex, buffer, 2, matches, 0) == 0) {
            // extract filename from request and decode URL
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);

            // get file extension
            char file_ext[32];
            strcpy(file_ext, get_file_extension(file_name));

            // build HTTP response
            char *response = (char *)malloc(MAX_LENGTH * 2 * sizeof(char));
            size_t response_len;
            build_http_response(file_name, file_ext, response, &response_len);

            // send HTTP response to client
            send(client_fd, response, response_len, 0);

            free(response);
            free(file_name);
        }
        regfree(&regex);
    }
    close(client_fd);
    free(arg);
    free(buffer);
    return NULL;
}*/

void *handle_client(void *arg) {
    printf("Client connected\n");
    int client_fd = *(int *) arg;
    // regex_t regex;
    // regcomp(&regex, "GET localhost\?([a-z0-9]+)=(.+)(&([a-z0-9]+)=(.+))*", REG_EXTENDED);

    char *buffer = (char *)malloc(MAX_LENGTH * sizeof(char));
    strcat(buffer, "Hello from client");
    ssize_t bytes_received;
    if ((bytes_received = recv(client_fd, buffer, MAX_LENGTH, 0)) < 0) {
        fprintf(stderr, "Error receiving data from client: %s\n", strerror(errno));
    }
    if (bytes_received > 0) {
        printf("BUFFER: %s\n", buffer);
        send(client_fd, &buffer, strlen(buffer) * sizeof(char), 0);
    }
    //Send string on buffer to client

    //Shut down socket gracefully
    shutdown(client_fd, SHUT_WR);
    //Finally, close socket
    close(client_fd);
    free(buffer);
    free(arg);
    return NULL;
}

int run_server(int port) {
    const int debug = is_debug();
    int server_fd, client_fd;
    //TODO child processes
    pid_t pid;
    //What is sent to the client
    char sent_buffer[MAX_LENGTH];
    socklen_t client_address_length;
    struct sockaddr_in server_address, client_address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        fprintf(stderr, "Socket failed with code %d: %s\n", errno, strerror(errno));
        return 1;
    }

    //Memset to clear the junk values stored in the memory address, just in case
    // memset(&server_address, 0, sizeof(server_address));
    if (get_file_contents(port, sent_buffer) != 0) {
        fprintf(stderr, "Failed to read file contents. Aborting...\n");
        return 1;
    }

    //**Set the server information**
    //AF_INET means it's using IPv4
    server_address.sin_family = AF_INET;
    //Allow all incoming addresses
    server_address.sin_addr.s_addr = SO_REUSEADDR;
    //Convert port to big endian ordering (most significant value first)
    server_address.sin_port = htons(port);

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
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        fprintf(stderr, "Listen failed with code %d: %s\n", errno, strerror(errno));
        return 1;
    }

    printf("Server running on port %d...waiting for connections.\n", port);

    while(1) {
        client_address_length = sizeof(client_address);
        if ((client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_address_length)) < 0) {
            fprintf(stderr, "Accept failed with code %d: %s\n", errno, strerror(errno));
            return 1;
        }
        char *ip = inet_ntoa(client_address.sin_addr);
        printf("%s from ip %s\n", "Received request...", ip);

        if (debug)
            printf("sending \"%s\"\n", sent_buffer); {
        }
        pid = fork(); //TODO
        if (pid < 0) {
            fprintf(stderr, "Fork failed with code %d: %s\n", errno, strerror(errno));
            shutdown(client_fd, SHUT_WR);
            close(client_fd);
            return 1;
        }
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, (void*) handle_client, (void *)client_fd);
        pthread_detach(thread_id);
        // char *buffer = (char *)malloc(MAX_LENGTH * sizeof(char));
        // ssize_t bytes_received = recv(client_fd, buffer, MAX_LENGTH, 0);
        // if (bytes_received > 0) {
        //     printf("BUFFER: %s\n", buffer);
        // }
        // //Send string on buffer to client
        // send(client_fd, &sent_buffer, strlen(sent_buffer) * sizeof(char), 0);
        // //Shut down socket gracefully
        // shutdown(client_fd, SHUT_WR);
        // //Finally, close socket
        // close(client_fd);
        // free(buffer);
    }
    //close listening socket
    close(server_fd);

    return 0;
}
