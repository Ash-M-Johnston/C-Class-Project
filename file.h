#ifndef FILE_H
#define FILE_H

typedef enum {
    FIRST,
    SECOND,
    THIRD
} FileType;

void get_file_contents(int selected, char* buffer);

#endif //FILE_H
