#include "file.h"
#include "server.h"
#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UNKNOWN 0
#define PLAIN_TEXT 1
#define HTML_TEXT 2
#define PNG_IMAGE 3
#define JPEG_IMAGE 4

void get_file_extension(ResponseInfo *response_info) {
    //find first instance of '.' in the string
    char *dot = strrchr(response_info->file_path, '.');
    if (!dot || dot == response_info->file_path) {
        response_info->extension = "";
        return;
    }
    response_info->extension = dot + 1;
}

char* char_mime_type(const ResponseInfo *response_info) {
    switch (response_info->mime_type) {
        case PLAIN_TEXT:
            return "text/plain";
        case HTML_TEXT:
            return "text/html";
        case PNG_IMAGE:
            return "image/png";
        case JPEG_IMAGE:
            return "image/jpeg";
        default:
            //MIME type for unknown data
            return "application/octet-stream";
    }
}

void store_mime_type(ResponseInfo *info) {
    get_file_extension(info);
    if (strcasecmp(info->extension, "txt") == 0) {
        info->mime_type = PLAIN_TEXT;
    } else if (strcasecmp(info->extension, "html") == 0) {
        info->mime_type = HTML_TEXT;
    } else if (strcasecmp(info->extension, "jpeg") == 0 || strcasecmp(info->extension, "jpg") == 0) {
        info->mime_type = JPEG_IMAGE;
    } else if (strcasecmp(info->extension, "png") == 0) {
        info->mime_type = PNG_IMAGE;
    } else {
        info->mime_type = UNKNOWN;
    }
}

void build_response(char *response_buffer, ResponseInfo *info, unsigned int *buffer_length) {
    char *header = malloc(MAX_LENGTH * sizeof(char));
    store_mime_type(info);
    snprintf(header, MAX_LENGTH,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", char_mime_type(info));
    printf("Opening file at location %s", info->file_path);
    FILE *file = fopen(info->file_path, "r");

    // Respond with 404 if file doesn't exist
    if (file == NULL) {
        /*long bytes_read;
        while ((bytes_read = read(fileno(file),response_buffer + *buffer_length,
                                MAX_LENGTH - *buffer_length)) > 0) {
            *buffer_length += bytes_read;
        }*/
        snprintf(response_buffer, MAX_LENGTH,
         "HTTP/1.1 404 Not Found\r\n"
         "Content-Type: text/plain\r\n"
         "\r\n"
         "404 Not Found");
        *buffer_length = strlen(response_buffer);
        return;
    }

    // copy header to response buffer
    *buffer_length = 0;
    memcpy(response_buffer, header, strlen(header));
    *buffer_length += strlen(header);

    long bytes_read;
    while ((bytes_read = read(fileno(file),response_buffer + *buffer_length,MAX_LENGTH - *buffer_length)) > 0) {
        *buffer_length += bytes_read;
    }

    free(header);
    fclose(file);
}

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
    const char *path = files_path();
    char filename[sizeof(path) + 100];
    strcpy(filename, path);
    //Is malloced earlier
    free(&path);
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
