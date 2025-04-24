#ifndef FILE_H
#define FILE_H

typedef struct {
    int is_text:1;
    int type:3;
    char *filename;
    char *html_type;
} SentFile;

int get_file_contents(int selected, char* buffer);

SentFile fd_from_type(int type);

#endif //FILE_H
