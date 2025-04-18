#include "helpers.h"
#include "tts.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>


void convert(const char *model) {
    char command[1024];
    
    // Create output directory if it doesn't exist
    mkdir("../output", 0777);

    DIR *dir = opendir("../input");
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
            continue;

        // Strip file extension (e.g., "input.txt" → "input")
        char basename[256];
        strncpy(basename, filename, sizeof(basename));
        char *dot = strrchr(basename, '.');
        if (dot) *dot = '\0';

        // Build command
        snprintf(command, sizeof(command),
            "piper-tts --model %s --output_file ../output/%s.wav < ../input/%s",
            model, basename, filename);

        // Run command
        int result = system(command);
        if (result == 0) {
            printf("Successfully converted %s\n", filename);
        } else {
            printf("Failed to convert %s\n", filename);
        }
    }

    closedir(dir);
}


void run() {
    char input[200] = "null";

    print_voices();
    while (1) {
        printf("Please enter the voice that you would like to use. Otherwise type 'exit'.\n"
                "Remember to type it exactly as it is listed (i.e. 'jenny_dioco (medium)') with no extra spaces\n");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Strip newline
        const char *filepath = get_path_from_voice(input);

        if (compare_strings_case_insensitive(input, "exit")) {
            return;
        } else if (!compare_strings_case_insensitive(filepath, "null")) {
            printf("Valid input: %s\n", input);
            convert(filepath);
            return;
        }
        printf("Invalid input. Please enter a valid voice or 'exit'\n\n");
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
            printf("Running Glowing Umbrella\n");
            run();
            return;
        } else if (compare_strings_case_insensitive(command, "Exit") == true) {
            return;
        } else {
            printf("Oops! Haven't heard that one in a while. Please use either \"run\" or \"exit\"\n");
        }
    }
}

void welcome() {
    printf("Welcome to Glowing Umbrella!\n"
        "A free text-to-speech application using Artificial Intelligence.\n"
        );
}

void print_working_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }
}

int main(int argc, char *argv[]) {
    print_working_directory();
    welcome();
    menu();
    printf("See you later!\n");
    return 0;
}