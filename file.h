#ifndef FILE_H
#define FILE_H

typedef enum {
    TEXT_FILE,
    IMAGE,
    HTML_FILE
} FileType;

int get_file_contents(FileType selected, char* buffer);

#endif //FILE_H
