#include "file.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//todo pass the file location to here instead of this jank

const FileDescriptor TEXT_FILE = {1, 0, "plain_text.txt", "text/plain" };
const FileDescriptor IMAGE =  {0, 1, "image.png", "image/png" };
const FileDescriptor HTML_FILE = {1, 2, "html_example.html", "text/html" };

FileDescriptor fd_from_type(int type) {
    FileDescriptor fd;
    switch (type) {
        case 0:
            fd = TEXT_FILE;
            break;
        case 1:
            fd = IMAGE;
            break;
        case 2:
            fd = HTML_FILE;
            break;
        default:
            fprintf(stderr, "Unknown FileDescriptor type: %d. Using default...\n", type);
            fd = TEXT_FILE;
            break;
    }
    return fd;
}

int get_file_contents(int selected, char* buffer) {
    char filename[100] = "../files/";
    FileDescriptor fd = fd_from_type(selected);
    strcat(filename, fd.filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file at location \"%s\".\n", filename);
        return 1;
    }
    //Copy to buffer
    char temp[MAX_LENGTH];
    // snprintf(buffer, "%s", temp);
    //Build HTML information
    strcat(buffer,"HTTP/1.1 200 OK\r\nContent-Type:");
    strcat(buffer, fd.html_type);
    strcat(buffer, "\r\n\r\n");

    while (fgets(temp, MAX_LENGTH, file)) {
        strcat(buffer, temp);
    }
    fclose(file);
    return 0;
}
