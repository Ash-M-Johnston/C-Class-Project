#ifndef FILE_H
#define FILE_H



typedef struct {
    int is_text:1;
    int type;
    char *filename;
    char *html_type;
} FileDescriptor;

int get_file_contents(int selected, char* buffer);

FileDescriptor fd_from_type(int type);

#endif //FILE_H
