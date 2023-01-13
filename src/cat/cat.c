#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    if(argc == 1){
        fputs("cat: missing operand\n"\
                 "Try 'cat -h' for more information\n", stderr);
        return EXIT_FAILURE;
    }
    int flagsexist = 0;
    if(argv[1][0] == '-'){
        for(int pos = 1; pos < strlen(argv[1]); pos++){
            switch(argv[1][pos]){
                case 'h':
                    fputs("cat [-flags] <filepath 1>...: Flags:\n"\
                                "    -h - help\n", stderr);
                    return EXIT_FAILURE;
                default:
                    fputs("cat: unknown flag, please type -h for help\n", stderr);
                    return EXIT_FAILURE;
            }
        }
    }
    for(int pos = 1; pos < argc; pos++){
        FILE *file = fopen(argv[pos], "r");
        if(access(argv[pos], F_OK))
            perror("cat");
        char ch;
        while((ch = fgetc(file)) != EOF)
            putchar(ch);
        fclose(file);
    }
}