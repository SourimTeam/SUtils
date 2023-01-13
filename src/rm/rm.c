#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <fts.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int recursive_remove(const char *dir)
{
    int ret = 0;
    FTS *ftsp = NULL;
    FTSENT *curr;
    char *files[] = { (char *) dir, NULL };
    ftsp = fts_open(files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
    if(!ftsp){
        fprintf(stderr, "%s: fts_open failed: %s\n", dir, strerror(errno));
        ret = -1;
        goto finish;
    }
    while((curr = fts_read(ftsp))){
        switch (curr->fts_info){
            case FTS_NS:
            case FTS_DNR:
            case FTS_ERR:
                fprintf(stderr, "rm: %s: fts_read error: %s\n",
                        curr->fts_accpath, strerror(curr->fts_errno));
                break;
            case FTS_DC:
            case FTS_DOT:
            case FTS_NSOK:break;
            case FTS_D:break;
            case FTS_DP:
            case FTS_F:
            case FTS_SL:
            case FTS_SLNONE:
            case FTS_DEFAULT:
                if (remove(curr->fts_accpath) < 0) {
                    fprintf(stderr, "rm: %s: Failed to remove: %s\n",
                            curr->fts_path, strerror(curr->fts_errno));
                    ret = -1;
                }
                break;
        }
    }
finish:
    if (ftsp) {
        fts_close(ftsp);
    }
    return ret;
}
void show_help(){
    puts("rm [-flags] <filepath 1>...: Flags:\n"\
                    "    -h - help\n"\
                    "    -f - ignore nonexistent files and arguments, never prompt\n"\
                    "    -r - remove directories and their contents recursive");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
    if(argc == 1){
        fputs("rm: missing operand\n"\
                 "Try 'rm -h' for more information\n", stderr);
        return EXIT_FAILURE;
    }

    int flagsexists = 0;
    int flagrecursive = 0;
    int flagforce = 0;

    if(argv[1][0] == '-'){
        flagsexists = 1;
        for(size_t pos = 1; pos < strlen(argv[1]); pos++){
            switch(argv[1][pos]){
                case 'r':
                    flagrecursive = 1;
                    break;
                case 'f':
                    flagforce = 1;
                    break;
                case 'h':show_help();
                default:
                    fputs("rm: unknown flag, please type -h for help\n", stderr);
                    return EXIT_FAILURE;
            }
        }
        if(argc == 2){
            fputs("'filepath' argument not specified\n"\
						   "Try 'rm -h' for more information\n", stderr);
            return EXIT_FAILURE;
        }
    }
    char *path = argv[(flagsexists) ? 2 : 1];
    if(!flagforce && access(path, F_OK))
        perror("rm");
    struct stat typefile;
    stat(path, &typefile);
    if(S_ISDIR(typefile.st_mode)){
        if(!flagrecursive){
            fputs("rm: is directory\n", stderr);
            return EXIT_FAILURE;
        }
        recursive_remove(path);
    } else {
        remove(path);
    }
    return 0;
}