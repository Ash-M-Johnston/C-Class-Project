#include "file.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum {
    PLAIN_TEXT = 0,
    HTML_TEXT = 1,
    PNG_IMAGE = 2,
    JPEG_IMAGE = 3
} FileVariant;

SentFile fd_from_type(int type) {
    SentFile sent_file;
    switch (type) {
        case PLAIN_TEXT:
            sent_file.filename = "plain_text.txt";
            sent_file.html_type = "text/html";
            sent_file.type = PLAIN_TEXT;
            sent_file.is_text = 1;
            break;
        case HTML_TEXT:
            sent_file.filename = "html_example.html";
            sent_file.html_type = "text/html";
            sent_file.type = HTML_TEXT;
            sent_file.is_text = 1;
            break;
        case PNG_IMAGE:
            sent_file.filename = "image.png";
            sent_file.html_type = "image/png";
            sent_file.type = PNG_IMAGE;
            sent_file.is_text = 0;
        break;
        case JPEG_IMAGE:
            sent_file.filename = "image.jpg";
            sent_file.html_type = "image/jpeg";
            sent_file.type = JPEG_IMAGE;
            sent_file.is_text = 0;
            break;
        default:
            // fprintf(stderr, "Unknown FileDescriptor type: %d. Using plain text...\n", type);
            SentFile sent4 = {1, 0, "plain_text.txt", "text/plain" };
        sent_file = sent4;
            break;
    }
    return sent_file;
}

int get_file_contents(int selected, char* buffer) {
    char filename[100] = "../files/";
    SentFile sent_file = fd_from_type(selected);
    strcat(filename, sent_file.filename);
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
    strcat(buffer, sent_file.html_type);
    strcat(buffer, "\r\n\r\n");

    while (fgets(temp, MAX_LENGTH, file)) {
        strcat(buffer, temp);
    }
    fclose(file);
    return 0;
}
