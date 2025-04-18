#include "helpers.h"
#include "tts.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


void convert() {
    // Prepare your command string
    const char *text = "Hello, this is Piper speaking.";
    const char *model = "en_GB-somevoice.onnx";
    const char *output = "output.wav";
    char command[512];

    // Build the command: echo "text" | piper --model ... --output_file ...
    snprintf(command, sizeof(command),
                "echo \"%s\" | ./piper --model %s --output_file %s",
                text, model, output);

    // Run the command
    int result = system(command);

    if (result == 0) {
        // Success: output.wav now contains the spoken text
        printf("Success with Piper\n");
    } else {
        // Handle error
        printf("Error with Piper\n");
    }
}


void run() {
    char voice[200] = "null";
    print_voices();
    while (1) {
        printf("Please enter the voice that you would like to use. Otherwise type 'exit'.\n"
                "Remember to type it exactly as it is listed (i.e. 'jenny_dioco (medium)') with no extra spaces\n");
        fgets(voice, sizeof(voice), stdin);
        voice[strcspn(voice, "\n")] = 0; // Strip newline

        if (compare_strings_case_insensitive(voice, "exit")) {
            return;
        } else if (!compare_strings_case_insensitive(get_path_from_voice(voice), "null")) {
            printf("Valid input: %s\n", voice);
            convert();
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


int main(int argc, char *argv[]) {
    welcome();
    menu();
    printf("See you later!\n");
    return 0;
}