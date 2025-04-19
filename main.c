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

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path, mode) mkdir(path, mode)
#endif



// Split a longer text file into chunks
void split_txt_file() {
    char filename[256];
    char fullpath[512];
    char base[256];

    // Prompt for file name (accept spaces)
    while (1) {
        printf("Enter the name of the .txt file in the input folder? (with NO SPACES in the file name!): ");
        if (!fgets(filename, sizeof(filename), stdin)) continue;
        filename[strcspn(filename, "\n")] = 0; // Remove newline
        if (strstr(filename, ".txt") == NULL) {
            printf("File must have a .txt extension. Please make sure to type .txt\n");
            continue;
        }
        snprintf(fullpath, sizeof(fullpath), "input/%s", filename);
        if (filepath_is_valid(fullpath)) break;
        printf("File not found. Try again.\n");
    }

    // Prompt for number of words per file
    int words_per_file = (int)get_numerical_input_in_range("How many words per split file?", 1, 1000000);

    // Open input file
    FILE *in = fopen(fullpath, "r");
    if (!in) {
        printf("Could not open file.\n");
        return;
    }

    get_base_filename(filename, base, sizeof(base));

    int file_idx = 1, word_count = 0;
    char word[128];
    char outname[512];
    FILE *out = NULL;

    while (fscanf(in, "%127s", word) == 1) {
        if (word_count % words_per_file == 0) {
            if (out) fclose(out);
            snprintf(outname, sizeof(outname), "input/%s__%d.txt", base, file_idx++);
            out = fopen(outname, "w");
            if (!out) {
                printf("Failed to create output file: %s\n", outname);
                fclose(in);
                return;
            }
        }
        fprintf(out, "%s", word);
        word_count++;
        // Add a space unless it's the last word in the file or chunk
        if (word_count % words_per_file != 0) {
            fprintf(out, " ");
        } else {
            fprintf(out, "\n");
        }
    }
    if (out) fclose(out);
    fclose(in);

    printf("Done splitting '%s' into %d-word files.\n", filename, words_per_file);
}

// Generate extensive english voice samples
void generate_voice_samples() {
    const char* input_file = "input/DO NOT DELETE/DO NOT DELETE.txt";
    char command[4096];
    
    // Verify input file exists
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        printf("Error: Input file '%s' not found!\n", input_file);
        return;
    }
    fclose(fp);

    // Create output directory
    if (MKDIR("output/samples", 0777) == 0) {
        printf("Directory created.\n");
    } else {
        printf("Directory may already exist or creation failed.\n");
    }

    // Voice parameters
    double length_scales[] = {0.5, 0.75, 1.00};
    double sentence_silences[] = {0.05, 0.1, 0.2};
    const int num_scales = sizeof(length_scales)/sizeof(length_scales[0]);
    const int num_silences = sizeof(sentence_silences)/sizeof(sentence_silences[0]);

    // Iterate through all English voices
    for (int i = 0; i < piper_voices_count; i++) {
        // Skip non-English voices
        if (strncmp(piper_voices[i].language_code, "en", 2) != 0) continue;
        
        const char* model_path = piper_voices[i].filepath;

        // Get base model name from filepath
        char base_model[256];
        get_base_filename(model_path, base_model, sizeof(base_model));

        // Generate all parameter combinations
        for (int ls = 0; ls < num_scales; ls++) {
            for (int ss = 0; ss < num_silences; ss++) {
                // Build output filename
                char output_file[256];
                snprintf(output_file, sizeof(output_file),
                    "output/samples/%s_ls%.2f_ss%.2f.wav",
                    base_model, length_scales[ls], sentence_silences[ss]);

                #ifdef _WIN32
                    snprintf(command, sizeof(command),
                    "bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"%s\" | E:\\Git\\glowing-umbrella\\piper_win\\piper.exe --model \"E:\\Git\\glowing-umbrella\\%s\" --length_scale %.2f --sentence_silence %.2f --output_file \"E:\\Git\\glowing-umbrella\\%s\"",
                    input_file, model_path, length_scales[ls], sentence_silences[ss], output_file);
                #else
                    snprintf(command, sizeof(command),
                        "piper-tts --model \"%s\" --length_scale %.2f --sentence_silence %.2f "
                        "--output_file \"output/%s.wav\" < \"input/%s\"",
                        model_path, length_scales[ls], sentence_silences[ss], output_file, input_file);
                #endif


                // Execute and show status
                // printf("Generating: %s\n", output_file);
                printf("Command: %s\n", command);
                int result = system(command);
                if (result != 0) {
                    printf("ERROR generating %s\n", output_file);
                }
            }
        }
    }
}

// Convert all files in the input folder into audio
void convert(const char *model) {
    char command[2048];
    DIR *dir = opendir("input");
    if (!dir) {
        perror("opendir");
        return;
    }

    double length_scale = get_numerical_input_in_range(
        "Please enter the scale (<1.0 = faster, >1.0 = slower)", 0.1, 2
    );
    double sentence_silence = get_numerical_input_in_range(
        "Please enter the pause between sentences (in seconds)", 0.1, 2
    );

    // Create output directory
    if (MKDIR("output", 0777) == 0) {
        printf("Directory created.\n");
    } else {
        printf("Directory may already exist or creation failed.\n");
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) continue;

        // Get base filename
        char basename[256];
        get_base_filename(filename, basename, sizeof(basename));

        // Build command
        #ifdef _WIN32
            snprintf(command, sizeof(command),
            "bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"E:\\\\Git\\\\glowing-umbrella\\\\input\\\\%s\" | E:\\\\Git\\\\glowing-umbrella\\\\piper_win\\\\piper.exe --model \"E:\\\\Git\\\\glowing-umbrella\\\\%s\" --length_scale %.2f --sentence_silence %.2f --output_file \"E:\\\\Git\\\\glowing-umbrella\\\\output\\\\%s.wav\"'",
            filename, model, length_scale, sentence_silence, basename);
        #else
            snprintf(command, sizeof(command),
                "piper-tts --model \"%s\" --length_scale %f --sentence_silence %f "
                "--output_file \"output/%s.wav\" < \"input/%s\"",
                model, length_scale, sentence_silence, basename, filename);
        #endif

        // Run command
        print_working_directory();
        printf("Command: %s\n", command);
        int result = system(command);
        if (result == 0) {
            printf("Successfully converted '%s'\n", filename);
        } else {
            printf("Failed to convert '%s'\n", filename);
        }
    }
    closedir(dir);
}

// Ask which voice to use before moving to conversion
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
            printf("Input \'%s\' is Valid.\n", input);
            convert(filepath);
            return;
        }
        printf("Invalid input \'%s\'. Please enter a valid voice or 'exit'\n\n", input);
    }
}

// Display main menu
void menu() {
    char command[200] = "null";
    // Verify validity of command
    while (true) {
        // Menu options
        printf("Options (not case-sensitive)\n"
                "1) Convert (txt files in input folder)\n"
                "2) Generate (voice samples in output/samples)\n"
                "3) Split (text files in input folder)\n"
                "4) Exit (program)\n");
        scanf("%s", command);
        if (compare_strings_case_insensitive(command, "Convert")) {
            printf("Running Glowing Umbrella\n");
            run();
            return;
        } else if (compare_strings_case_insensitive(command, "Generate")) {
            generate_voice_samples();
        } else if (compare_strings_case_insensitive(command, "Split")) {
            split_txt_file();
        } else if (compare_strings_case_insensitive(command, "Exit")) {
            return;
        } else {
            printf("Oops! Haven't heard that one in a while. Please use either \"run\" or \"exit\"\n");
        }
    }
}

// Welcome text
void welcome() {
    printf("Welcome to Glowing Umbrella!\n"
        "A free text-to-speech application using Artificial Intelligence.\n"
        );
}



int main(int argc, char *argv[]) {
    print_working_directory();
    welcome();
    menu();
    printf("See you later!\n");
    return 0;
}