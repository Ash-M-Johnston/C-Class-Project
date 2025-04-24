#ifndef SERVER_H
#define SERVER_H

//Maximum length of the byte buffer to be sent to the client
#define MAX_LENGTH 314/*5728*/ //3 MBs
//Default port to host connection
#define DEFAULT_PORT 8081

#define MAX_CONNECTIONS 10

int run_server(int port);

#endif //SERVER_H
