#include "file.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
//todo pass the file location to here instead of this jank
int get_file_contents(const FileType selected, char* buffer) {
    char filename[100] = "../files/";
    switch (selected) {
        case TEXT_FILE:
            strcat(filename, "plain_text.txt");
            break;
        case IMAGE:
            strcat(filename, "second.txt");
            break;
        case HTML_FILE:
            strcat(filename, "html_example.html");
            break;
        default:
            fprintf(stderr, "Unknown selected number %d. Try again with 0-2.", selected);
            return 1;
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file at location \"%s\".\n", filename);
        return 1;
    }
    //Copy to buffer
    char temp[MAX_LENGTH];
    snprintf(buffer, "%s", temp);
    strcat(buffer,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    while (fgets(temp, MAX_LENGTH, file)) {
        strcat(buffer, temp);
    }
    fclose(file);
    return 0;
}
