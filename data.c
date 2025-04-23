#include "data.h"
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

// Numerical input with range validation
double get_numerical_input_in_range(const char *prompt, double min, double max) {
    char buf[100];
    char *endptr;
    double value;

    while (1) {
        printf("%s (between %.2f and %.2f): ", prompt, min, max);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        buf[strcspn(buf, "\n")] = 0;

        value = strtod(buf, &endptr);
        while (isspace((unsigned char)*endptr)) endptr++;

        if (buf[0] && *endptr == '\0' && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number between %.2f and %.2f.\n", min, max);
    }
}

// Case-insensitive string comparison
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