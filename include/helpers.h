#ifndef F3C2E071_BE5F_431E_A183_481A6B17B82E
#define F3C2E071_BE5F_431E_A183_481A6B17B82E

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Function to accept only numerical input
double get_numerical_input_in_range(const char *prompt, double min, double max) {
    char buf[100];
    char *endptr;
    double value;

    while (1) {
        printf("%s (between %.2f and %.2f): ", prompt, min, max);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        buf[strcspn(buf, "\n")] = 0; // Remove newline

        value = strtod(buf, &endptr);

        // Skip trailing whitespace
        while (isspace((unsigned char)*endptr)) endptr++;

        if (buf[0] && *endptr == '\0' && value >= min && value <= max) {
            return value;
        }
        printf("Please enter a valid number between %.2f and %.2f.\n", min, max);
    }
}

// Function to only 

// Compare two strings in a case-insensitive manner
bool compare_strings_case_insensitive(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return (str1 == str2);

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 != len2) return false;

    for (size_t i = 0; i < len1; i++) {
        if (tolower((unsigned char)str1[i]) != tolower((unsigned char)str2[i])) {
            return false;
        }
    }

    return true;
}

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
