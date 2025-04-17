#ifndef F3C2E071_BE5F_431E_A183_481A6B17B82E
#define F3C2E071_BE5F_431E_A183_481A6B17B82E

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Determine if filepath is valid
bool filepath_is_valid(const char *filepath) {
    FILE *fp;

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        printf("Invalid file path: \"%s\"\n", filepath);
        return false;
    }
    printf("Valid file path: \"%s\"\n", filepath);
    fclose(fp);
    return true;
}

// Determine if filepath contains a certain string
bool filepath_contains_txt(const char *filepath, const char *txt) {
    const char *txt_ptr = strstr(filepath, txt);
    if (txt_ptr == NULL) {
        printf("Filepath does not contain \"%s\"\n", txt);
        return false;
    }
    printf("Filepath contains \"%s\"\n", txt);
    return true;
}

#endif /* F3C2E071_BE5F_431E_A183_481A6B17B82E */
