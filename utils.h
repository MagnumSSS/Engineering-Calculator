#ifndef UTILS_H
#define UTILS_H

#include <stddef.h> // для size_t
#include "main.h"
#include <stdio.h>

extern FILE* output_file;


// В utils.h
#define LOG(...) do { \
    if (output_file) { \
        fprintf(output_file, __VA_ARGS__); \
        fflush(output_file); /* сразу пишем на диск */ \
    } \
} while(0)

void clear_stdin(void);
float number_user_input(const char* text);
void user_input(char* mod, size_t size);

#endif // UTILS_H
