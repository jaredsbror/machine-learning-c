#ifndef APPLICATION_H
#define APPLICATION_H

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

// Application logic
void split_txt_file(void);
void generate_voice_samples(void);
void convert(const char *model);
void convertCustom(const char *model_path, const char *fixed_input_path, const char *fixed_output_path, 
    const double length_scale, const double sentence_silence);
void run(void);
void menu(void);
void welcome(void);

#endif // APPLICATION_H


