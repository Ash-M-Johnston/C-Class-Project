#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int is_debug() {
    const char *env = getenv("DEBUG");
    if (env == NULL) {
        return 0;
    }
    int is_debug = strcmp(env, "true");
    if(strcmp(env, "1")) {
        is_debug = 1;
    }
    if (is_debug) {
        printf("DEBUG is set to true.\n");
        return 1;
    }
    return 0;
}

void printf_if_debug(char *fmt_str, ...) {
    if (!is_debug()) {
        return;
    }
    va_list args;
    for (va_start(args, fmt_str); *fmt_str != '\0'; fmt_str++) {
        switch (*fmt_str) {
            case 'd':
                printf("%d", va_arg(args, int));
                break;
            case 'c':
                printf("%c", va_arg(args, int));
                break;
            case 's':
                printf("%s", va_arg(args, char *));
                break;
            default:
                printf("%c", *fmt_str);
                break;
        }
    }
}


