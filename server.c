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
#include <signal.h>

void *client_connection(void *arg) {
    printf("Client connected\n");
    int client_fd = *(int *) arg;
    char *buffer = malloc(MAX_LENGTH * sizeof(char));
    //Read received buffer,
    ssize_t bytes = recv(client_fd, buffer, MAX_LENGTH, 0);
    if (bytes > 0) {
        regex_t regex;
        //Regex to check if the response is an HTML GET request
        regcomp(&regex, "GET /([^ ]*) HTTP/1.", REG_EXTENDED);
        //First index will be entire match, second will just be the file the client is requesting
        regmatch_t match[2];
        int result = regexec(&regex, buffer, 2, match, 0);
        //Execute regex
        if (result == 0) {
            printf("Match found\n");
            //Cut off buffer at the end of the found match
            buffer[match[1].rm_eo] = '\0';
            //Create new pointer at the start of the match
            char *filename = buffer + match[1].rm_so;

            //If nothing is specified, go to homepage
            if (strcmp(filename, "/") == 0 || strcmp(filename, "") == 0) {
                strcpy(filename, "home.html");
            }
            printf("Filename: %s\n", filename);

            unsigned int response_len;
            char *response_buffer = malloc(MAX_LENGTH * 2 * sizeof(char));
            ResponseInfo *response_info = malloc(sizeof(ResponseInfo));
            char *path = files_path();
            response_info->file_path = malloc(strlen(path) + strlen(filename) + 300);
            strcpy(response_info->file_path, path);
            strcat(response_info->file_path, filename);
            free(path);
            build_response(response_buffer, response_info, &response_len);
            send(client_fd, response_buffer, response_len, 0);
            free(response_buffer);
            free(response_info->file_path);
            free(response_info);
        }
        regfree(&regex);
    }
    //Finally, close socket
    close(client_fd);
    free(arg);
    free(buffer);
    return NULL;
}
//Global so sigint_handler can properly shut down the socket when the process is killed
int server_fd;
struct sigaction old_action;

void signal_handler(const int sig_num) {
    printf("Program terminated, safely closing the server...\n");
    sigaction(sig_num, &old_action, NULL);
    close(server_fd);
    kill(0, sig_num);
}

int run_server(unsigned int port) {
    socklen_t client_address_length;
    struct sockaddr_in server_address, client_address;

    //Makes sure the server properly shuts down when Ctrl + C is pressed
    struct sigaction action = {0};
    action.sa_handler = &signal_handler;
    sigaction(SIGTERM, &action, &old_action);
    sigaction(SIGINT, &action, &old_action);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        fprintf(stderr, "Socket failed to open with code %d: %s\n", errno, strerror(errno));
        return 1;
    }

    //**Set the server information**
    //AF_INET means it's using IPv4
    server_address.sin_family = AF_INET;
    //Allow all incoming addresses
    server_address.sin_addr.s_addr = INADDR_ANY;/*SO_REUSEADDR;*/
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
        int *client_fd = malloc(sizeof(int));
        if ((*client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_address_length)) < 0) {
            fprintf(stderr, "Accept failed with code %d: %s\n", errno, strerror(errno));
            close(server_fd);
            return 1;
        }
        printf("Received request from ip %s\n", inet_ntoa(client_address.sin_addr));

        //Creates a new thread instead of a new instance of this program with fork
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, (void*) client_connection, client_fd);
        pthread_detach(thread_id);
    }
    //close listening socket
    close(server_fd);

    return 0;
}
