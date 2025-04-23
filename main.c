#include "application.h"
#include "files.h"
#include "voices.h"
#include "data.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    print_working_directory();
    welcome();
    menu();
    printf("See you later!\n");
    return 0;
}