#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

// Input utilities
double get_numerical_input_in_range(const char *prompt, double min, double max);
bool compare_strings_case_insensitive(const char *str1, const char *str2);

#endif // DATA_H
