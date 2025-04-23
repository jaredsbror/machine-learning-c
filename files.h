#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

// Directory and file utilities
int mkdir_p(const char *dir, mode_t mode);
char* get_project_root(void);
void get_base_filename(const char* filename, char* base, size_t base_size);
bool filepath_is_valid(const char *filepath);
void print_working_directory(void);

#endif // FILES_H
