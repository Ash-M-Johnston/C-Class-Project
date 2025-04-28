#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


char *files_path() {
    char *env = getenv("PROJECT_FILES_PATH");
    if (env == NULL) {
        env = "../files/";
        /*debug_*/printf("PROJECT_FILES_PATH env variable not set, default value of %s will be used.\n", env);
    }
    char *path = malloc(strlen(env) + 200 * sizeof(char));
    strcpy(path, env);

    return path;
}

int is_debug() {
    const char *env = getenv("DEBUG");
    if (env == NULL) {
        return 0;
    }
    int is_debug = strcasecmp(env, "true");
    if(strcmp(env, "1") == 0) {
        is_debug = 1;
    }
    if (is_debug) {
        printf("DEBUG is set to true.\n");
        return 1;
    }
    return 0;
}

void debug_printf(char *format, ...) {
    unsigned int i;
    char *s;

    //initialize args
    va_list arg;
    va_start(arg, format);

    for (char *traverse = format; *traverse != '\0'; traverse++) {
        while (*traverse != '%') {
            putchar(*traverse);
            traverse++;
        }

        traverse++;

        //execute arguments
        switch (*traverse) {
            case 'c': i = va_arg(arg, int); //Fetch char argument
                putchar(i);
                break;

            case 'd': i = va_arg(arg, int); //Fetch Decimal/Integer argument
                if (i < 0) {
                    i = -i;
                    putchar('-');
                }
                printf("%d", i);
                break;

            case 's': s = va_arg(arg, char *); //Fetch string
                if (s == NULL) break;
                puts(s);
                break;
        }
    }

    //end vararg
    va_end(arg);
}
