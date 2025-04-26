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
    printf("MIME_TYPE val: %hi\n", response_info->mime_type);
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
    printf("Header info is: %s\n", char_mime_type(info));
    printf("Opening file at location %s\n", info->file_path);
    FILE *file = fopen(info->file_path, "r");

    // Respond with 404 if file doesn't exist
    if (file == NULL) {
        snprintf(response_buffer, MAX_LENGTH,
         "HTTP/1.1 404 Not Found\r\n"
         "Content-Type: text/html\r\n"
         "\r\n"
         "<h1>Error 404:</h1>\r\n"
         "<p>File %s not found.\r\n"
         "<br>\r\n"
         "<img src=\"sad.jpg\" alt=\"A sad face\" style=\"width:320px;height:320px;\">\"", info->file_path);
        *buffer_length = strlen(response_buffer);
        free(header);
        printf("Invalid file requested\n");
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