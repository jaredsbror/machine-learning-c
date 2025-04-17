#include "filepath.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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


void run() {
    char src_addr[200] = "null";
    char dest_addr[200] = "null";

    while (true) {
        printf("What is the source file path? (e.g. /home/desktop/item.txt)");
        scanf("%s", src_addr);
        if (filepath_is_valid(src_addr) && filepath_contains_txt(src_addr, ".txt")) break;
        // else printf("Invalid path \"%s\"", src_addr);
    }

    while (true) {
        printf("What is the destination directory? (e.g. /home/desktop/)");
        scanf("%s", dest_addr);
        if (filepath_is_valid(dest_addr) && filepath_contains_txt(src_addr, ".txt")) break;
        // else printf("Invalid path \"%s\"", dest_addr);
    }
}


void menu() {
    char command[200] = "null";
    // Verify validity of command
    while (true) {
        // Menu options
        printf("Options (not case-sensitive)\n"
                "1) Run\n"
                "2) Exit\n");
        scanf("%s", command);
        if (compare_strings_case_insensitive(command, "Run") == true) {
            printf("Running program\n");
            run();
            return;
        } else if (compare_strings_case_insensitive(command, "Exit") == true) {
            printf("See you later!\n");
            return;
        } else {
            printf("Oops! Haven't heard of that one. Please use either \"run\" or \"exit\"\n");
        }
    }
}

void welcome() {
    printf("Welcome to Glowing Umbrella!\n"
        "A free text-to-speech application using Artificial Intelligence.\n"
        );
}


int main(int argc, char *argv[]) {
    welcome();
    menu();
    return 0;
}