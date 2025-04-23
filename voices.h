#ifndef VOICES_H
#define VOICES_H

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

// PiperVoice struct definition
typedef struct {
    const char *language_code;
    const char *voice_name;
    const char *filepath;
} PiperVoice;

// Extern declaration of the voices array and its count
extern PiperVoice piper_voices[];
extern const int piper_voices_count;

// Function prototypes (declarations only)
void print_voices(void);
const char* get_path_from_voice(const char *voice);
// Used for name comparison
bool compare_strings_case_insensitive(const char *str1, const char *str2);

#endif // VOICES_H


