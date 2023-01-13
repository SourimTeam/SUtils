#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

short dirIsEmpty(const char *path){
    struct dirent *file;
    DIR *dir = opendir(path);
    int countfiles = 0;
    if(dir != NULL){
        while((file = readdir(dir)) != NULL){
            countfiles++;
        }
    } else  
        perror("rmdir");
    return countfiles <= 2;
}

int main(int argc, char **argv){
    if(argc == 1){
        fputs("rmdir: missing operand\n"\
                 "Try 'rmdir -h' for more information\n", stderr);
        return EXIT_FAILURE;
    }
    int flagsexist = 0;
    int flagparents = 0;
    if(argc > 1 && argv[1][0] == '-'){
        flagsexist = 1;
        for(int pos = 1; pos < strlen(argv[1]); pos++){
            switch(argv[1][pos]){
                case 'p':flagparents=1;break;
                case 'h':
                    puts("rmdir [-flags] <dir path>: Flags:\n"\
                          "    -h - show this message\n"\
                          "    -p - remove parent directories");
                    return EXIT_SUCCESS;
                default:
                    fputs("rmdir: unknown flag, please type -h for help\n", stderr);
                    return EXIT_FAILURE;
            }
        }
    }
    char *path = argv[(flagsexist) ? 2 : 1]; 
    if(access(path, F_OK))
        perror("rmdir");
    if(dirIsEmpty(path)){
        rmdir(path);
    } else {
        if(flagparents){
            char *temppath = malloc(strlen(path));
            strcat(temppath, path);
            for(size_t pos = strlen(path); pos > 0; pos--){
                if(path[pos] == '/' || path[pos] == '\\'){
                    rmdir(temppath);
                } else 
                    path[pos] = 0;
            }
        } else {
            fputs("rmdir: directory is not empty\n", stderr);
            return EXIT_FAILURE;
        }
    }
    return 0;
}