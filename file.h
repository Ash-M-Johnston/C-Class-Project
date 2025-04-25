#ifndef FILE_H
#define FILE_H

typedef struct {
    int is_text:1;
    int type:3;
    char *filename;
    char *html_type;
} SentFile;

typedef struct {
    char *file_path;
    char *extension;
    int mime_type:3;
} ResponseInfo;

void get_file_extension(ResponseInfo *response_info);

char* char_mime_type(const ResponseInfo *response_info);

void store_mime_type(ResponseInfo *info);

void build_response(char *response_buffer, ResponseInfo *info, unsigned int *buffer_length);

int get_file_contents(int selected, char* buffer);

SentFile fd_from_type(int type);

#endif //FILE_H
