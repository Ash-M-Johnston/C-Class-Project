#include "file.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

void get_file_contents(const int selected, char* buffer) {
    char filename[50] ="../";
    switch (selected) {
        case FIRST:
            strcat(filename, "first.txt");
            break;
        case SECOND:
            strcat(filename, "second.txt");
            break;
        case THIRD:
            strcat(filename, "third.txt");
            break;
        default:
            strcat(filename, "unknown.txt");
            printf("Unknown selected number %d. Try again with 0-2", selected);
            break;
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    while (fgets(buffer, MAX_LENGTH, file)) {}
    fclose(file);
}
