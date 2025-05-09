#include "application.h"
#include "files.h"
#include "voices.h"
#include "data.h"
#include <sys/stat.h>
#include <dirent.h>
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <stdio.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

// application.h
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void split_txt_file_gui(const char* txtPath, int wordsPerFile);
EXPORT void convertCustom(const char *model_path, const char *input_path, 
                         const char *output_path, double length_scale, 
                         double sentence_silence);
EXPORT void generate_voice_samples();


void split_txt_file_gui(const char* txtPath, int wordsPerFile) {
    FILE *in = fopen(txtPath, "r");
    if (!in) {
        MessageBoxW(NULL, L"Failed to open input file", L"Error", MB_ICONERROR);
        return;
    }

    // Extract directory and base filename without extension
    char dir[1024], base[256];
#ifdef _WIN32
    char drive[10], ext[256];
    _splitpath_s(txtPath, drive, sizeof(drive), dir, sizeof(dir), base, sizeof(base), ext, sizeof(ext));
    // Remove original extension if it's .txt
    if (_stricmp(ext, ".txt") == 0) {
        char *dot = strrchr(base, '.');
        if (dot) *dot = '\0';
    }
#else
    char *path_copy = strdup(txtPath);
    strncpy(dir, dirname(path_copy), sizeof(dir)-1);
    dir[sizeof(dir)-1] = '\0';
    free(path_copy);
    
    path_copy = strdup(txtPath);
    char *filename = basename(path_copy);
    strncpy(base, filename, sizeof(base)-1);
    base[sizeof(base)-1] = '\0';
    free(path_copy);
    
    // Remove .txt extension if present
    char *ext = strrchr(base, '.');
    if (ext && strcasecmp(ext, ".txt") == 0) {
        *ext = '\0';
    }
#endif

    int file_idx = 100, word_count = 0;
    char word[128];
    char outpath[2048];
    FILE *out = NULL;

    while (fscanf(in, "%127s", word) == 1) {
        if (word_count % wordsPerFile == 0) {
            if (out) fclose(out);
            
            // Build output path with single .txt extension
#ifdef _WIN32
            snprintf(outpath, sizeof(outpath), "%s\\%s__%d.txt", dir, base, file_idx++);
#else
            snprintf(outpath, sizeof(outpath), "%s/%s__%d.txt", dir, base, file_idx++);
#endif
            out = fopen(outpath, "w");
            
            if (!out) {
                MessageBoxW(NULL, L"Failed to create output file", L"Error", MB_ICONERROR);
                fclose(in);
                return;
            }
        }

        fprintf(out, "%s", word);
        word_count++;

        // Add space unless it's the last word in chunk
        if (word_count % wordsPerFile != 0) {
            fprintf(out, " ");
        } else {
            fprintf(out, "\n");
        }
    }

    if (out) fclose(out);
    fclose(in);
}
                        
                        


// Split a longer text file into chunks
void split_txt_file() {
    char *project_root = get_project_root();
    if (!project_root) return;

    char filename[256];
    char fullpath[512];
    char base[256];

    // Get input directory path
    char input_dir[512];
    snprintf(input_dir, sizeof(input_dir), "%s/input", project_root);

    while (1) {
        printf("Enter .txt filename in input folder (no spaces): ");
        if (!fgets(filename, sizeof(filename), stdin)) continue;
        filename[strcspn(filename, "\n")] = 0;
        
        // Build full input path
        snprintf(fullpath, sizeof(fullpath), "%s/input/%s", project_root, filename);
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

    int file_idx = 100, word_count = 0;
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
    char* project_root = get_project_root();
    if (!project_root) return;

    char input_path[4096];
    snprintf(input_path, sizeof(input_path), 
            "%s/input/DO NOT DELETE/DO NOT DELETE.txt", project_root);

    FILE *fp = fopen(input_path, "r");
    if (!fp) {
        perror(input_path);
        return;
    }
    fclose(fp);

    char output_dir[4096];
    snprintf(output_dir, sizeof(output_dir), 
            "%s/output/samples", project_root);
    
    struct stat st = {0};
    if (stat(output_dir, &st) == -1) {
        // Directory doesn't exist; create it
        if (mkdir_p(output_dir, 0755) != 0) {
            perror("Failed to create output directory");
            return;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Path exists but is not a directory: %s\n", output_dir);
        return;
    }

    double length_scales[] = {0.6, 0.7, 0.8, 0.9, 1.00};
    double sentence_silences[] = {0.05, 0.1, 0.15, 0.2, 0.25, 0.30};
    const int num_scales = sizeof(length_scales)/sizeof(length_scales[0]);
    const int num_silences = sizeof(sentence_silences)/sizeof(sentence_silences[0]);

    char command[4096];

    for (int i = 0; i < piper_voices_count; i++) {
        if (strncmp(piper_voices[i].language_code, "en", 2) != 0) continue;

        const char* model_path = piper_voices[i].filepath;
        char base_model[256];
        get_base_filename(model_path, base_model, sizeof(base_model));

        for (int ls = 0; ls < num_scales; ls++) {
            for (int ss = 0; ss < num_silences; ss++) {
                char output_file[256];
                snprintf(output_file, sizeof(output_file),
                        "%s_ls%.2f_ss%.2f.wav",
                        base_model, length_scales[ls], sentence_silences[ss]);
                
                char output_path[4096];
                snprintf(output_path, sizeof(output_path),
                        "%s/%s", output_dir, output_file);

            #ifdef _WIN32
                snprintf(command, sizeof(command),
                    "bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"%s\" | \"%s/piper_win/piper.exe\""
                    " --model \"%s/%s\" --length_scale %.2f --sentence_silence %.2f"
                    " --output_file \"%s\"",
                    input_path, project_root, 
                    project_root, model_path,
                    length_scales[ls], sentence_silences[ss],
                    output_path);
            #else
                snprintf(command, sizeof(command),
                    "piper-tts --model \"%s/%s\" --length_scale %.2f --sentence_silence %.2f"
                    " --output_file \"%s\" < \"%s\"",
                    project_root, model_path,
                    length_scales[ls], sentence_silences[ss],
                    output_path, input_path);
            #endif

                printf("Executing: %s\n", command);
                int result = system(command);
                if (result != 0) {
                    printf("ERROR generating %s\n", output_path);
                }
            }
        }
    }
}

// Convert all files in the input folder into audio
void convertCustom(const char *model_path, const char *fixed_input_path, const char *fixed_output_path, 
    const double length_scale, const double sentence_silence) {
    char* project_root = get_project_root();
    if (!project_root) return;

    char command[4096];

    // Use parameterized input path
    DIR *dir = opendir(fixed_input_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    // Use parameterized output path
    mkdir_p(fixed_output_path, 0755);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) continue;

        char basename[256];
        get_base_filename(filename, basename, sizeof(basename));

        // Build paths using parameters
        char input_path[4096];
        snprintf(input_path, sizeof(input_path), "%s/%s", fixed_input_path, filename);

        char output_path[4096];
        snprintf(output_path, sizeof(output_path), "%s/%s.wav", fixed_output_path, basename);

        #ifdef _WIN32
            snprintf(command, sizeof(command),
            "bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"%s\" | \"%s/piper_win/piper.exe\""
            " --model \"%s\" --length_scale %.2f --sentence_silence %.2f"
            " --output_file \"%s\"",
            input_path, project_root,
            model_path,
            length_scale, sentence_silence,
            output_path);
        #else
            snprintf(command, sizeof(command),
            "piper-tts --model \"%s\" --length_scale %.2f --sentence_silence %.2f"
            " --output_file \"%s\" < \"%s\"",
            model_path,
            length_scale, sentence_silence,
            output_path, input_path);
        #endif

        printf("Processing: %s\n", filename);
        int result = system(command);
        if (result == 0) {
            printf("Success: %s\n", output_path);
        } else {
            printf("Failed: %s\n", filename);
        }
    }
    closedir(dir);
}



// Convert all files in the input folder into audio
void convert(const char *model) {
    char* project_root = get_project_root();
    if (!project_root) return;

    char command[4096];
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

    char output_dir[4096];
    snprintf(output_dir, sizeof(output_dir), "%s/output", project_root);
    mkdir_p(output_dir, 0755);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) continue;

        char basename[256];
        get_base_filename(filename, basename, sizeof(basename));

        char input_path[4096];
        snprintf(input_path, sizeof(input_path), "%s/input/%s", project_root, filename);
        
        char output_path[4096];
        snprintf(output_path, sizeof(output_path), "%s/%s.wav", output_dir, basename);

    #ifdef _WIN32
        snprintf(command, sizeof(command),
            "bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"%s\" | \"%s/piper_win/piper.exe\""
            " --model \"%s/%s\" --length_scale %.2f --sentence_silence %.2f"
            " --output_file \"%s\"",
            input_path, project_root, 
            project_root, model,
            length_scale, sentence_silence,
            output_path);
    #else
        snprintf(command, sizeof(command),
            "piper-tts --model \"%s/%s\" --length_scale %.2f --sentence_silence %.2f"
            " --output_file \"%s\" < \"%s\"",
            project_root, model,
            length_scale, sentence_silence,
            output_path, input_path);
    #endif

        printf("Processing: %s\n", filename);
        int result = system(command);
        if (result == 0) {
            printf("Success: %s\n", output_path);
        } else {
            printf("Failed: %s\n", filename);
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