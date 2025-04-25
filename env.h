#ifndef ENV_H
#define ENV_H

int is_debug();

char *files_path();

void printf_if_debug(char *fmt_str, ...);

void debug_printf(char *format, ...);
#endif //ENV_H
