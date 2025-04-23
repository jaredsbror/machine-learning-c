#include "files.h"
#include "voices.h"
#include "data.h"
#include <sys/stat.h>
#include <dirent.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

// Directory creation with parent directories
int mkdir_p(const char *dir, mode_t mode) {
    char tmp[4096];
    char *p = NULL;
    size_t len;
    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            MKDIR(tmp, mode);
            *p = '/';
        }
    return MKDIR(tmp, mode);
}

// Get project root path
char* get_project_root() {
    static char path_buffer[4096];
    if (!getcwd(path_buffer, sizeof(path_buffer))) {
        perror("getcwd failed");
        return NULL;
    }
    char* found = strstr(path_buffer, "glowing-umbrella");
    if (!found) {
        fprintf(stderr, "Project root not found in path: %s\n", path_buffer);
        return NULL;
    }
    *(found + strlen("glowing-umbrella")) = '\0';
    return path_buffer;
}

// Get base filename (handles Windows paths)
void get_base_filename(const char* filename, char* base, size_t base_size) {
    const char *slash = strrchr(filename, '/');
#ifdef _WIN32
    const char *bslash = strrchr(filename, '\\');
    if (!slash || (bslash && bslash > slash)) slash = bslash;
#endif
    const char *start = slash ? slash + 1 : filename;
    strncpy(base, start, base_size - 1);
    base[base_size - 1] = '\0';
    char *dot = strrchr(base, '.');
    if (dot) *dot = '\0';
}

// Check if a file exists
bool filepath_is_valid(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

// Print working directory
void print_working_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }
}