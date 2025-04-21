#include "file.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

int get_file_contents(const FileType selected, char* buffer) {
    char filename[50] ="../files/";
    switch (selected) {
        case FIRST_FILE:
            strcat(filename, "first.txt");
            break;
        case SECOND_FILE:
            strcat(filename, "second.txt");
            break;
        case THIRD_FILE:
            strcat(filename, "third.txt");
            break;
        default:
            fprintf(stderr, "Unknown selected number %d. Try again with 0-2.", selected);
            return 1;
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file, attempting workaround...\n");
        char *ptr = &filename[0];
        file = fopen(ptr, "r");
        if (file == NULL) {
            printf("Workaround failed.\n");
            return 1;
        }
    }
    //Copy to buffer
    char temp[MAX_LENGTH];
    while (fgets(temp, MAX_LENGTH, file)) {
        strcat(buffer, temp);
    }
    fclose(file);
    return 0;
}
