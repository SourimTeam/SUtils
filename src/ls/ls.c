#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
    printf("\x1b[1m");
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    DIR *dir;
    struct dirent *odir;
    int flagsexist = 0;
    int fall = 0;
    if(argc > 1 && argv[1][0] == '-'){
        flagsexist = 1;
        for(int pos = 1; pos < strlen(argv[1]); pos++){
            switch(argv[1][pos]){
                case 'a':
                    fall = 1;
                    break;
                case 'h':
                    fputs("ls [-flags] [dir path]: Flags:\n"\
                             "    -h - show this message\n"\
                             "    -a - show show hidden files\n", stderr);
                    return EXIT_FAILURE;
                default:
                    fputs("ls: unknown flag, please type -h for help\n", stderr);
                    return EXIT_FAILURE;
            }
        }
    }
    if(argc > 1 || (flagsexist && argc > 2)){
        char *path = argv[(flagsexist) ? 2 : 1]; 
        if(access(path, F_OK))
            perror("ls");
        chdir(path);
    }
    dir = opendir("./");
    size_t maxlen = 0;
    if(dir != NULL){
        while((odir = readdir(dir)) != NULL){
            if(strlen(odir->d_name) > maxlen)
                maxlen = strlen(odir->d_name);
        }
        closedir(dir);
    } else  
        perror("rm");
    dir = opendir("./");
    size_t step = 0;
    if(dir != NULL){
        while((odir = readdir(dir)) != NULL){
            if(odir->d_name[0] == '.' && !fall)
                continue;
            switch(odir->d_type){
                case DT_DIR:
                    printf("\x1b[34m");
                    break;
            }
            printf("%s", odir->d_name);
            for(size_t pos = 0; pos < (maxlen-strlen(odir->d_name))+2; pos++)
                putchar(' ');
            if(step >= window.ws_col/(maxlen+5+strlen(odir->d_name))){
                step = 0;
                putchar('\n');
            } else
                step++;
        }
        closedir(dir);
    } else
        perror("ls");
    printf("\x1b[0m");
    if(step > 0)
        putchar('\n');
    return 0;
}