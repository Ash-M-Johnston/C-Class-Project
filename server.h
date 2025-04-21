#ifndef SERVER_H
#define SERVER_H

//Maximum length of the byte buffer to be sent to the client
#define MAX_LENGTH 8192
//Default port to host connection
#define SERVER_PORT 8081

#define MAX_CONNECTIONS 10

int run_server(int var1);

#endif //SERVER_H
