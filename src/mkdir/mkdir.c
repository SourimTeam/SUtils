#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void showError(const char *msg){
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
	if(argc == 1){
        fputs("mkdir: missing operand\n"\
                 "Try 'mkdir -h' for more information\n", stderr);
        return EXIT_FAILURE;
    }
	int flagsexist = 0;
	int fparents = 0;
	if(argc > 1 && argv[1][0] == '-'){
		flagsexist = 1;
		char *flag = argv[1];
		for(size_t pos = 1; pos < strlen(flag); pos++){
			switch(flag[pos]){
				case 'p':
					fparents = 1;
					break;
				case 'h':showError("mkdir [-flags] <dir name 1>...: Flags:\n"\
										"    -h - show this message\n"\
									    "    -p - no error if existing, make parent directories as needed\n");
				default:
                    fputs("mkdir: unknown flag, please type -h for help\n", stderr);
                    return EXIT_FAILURE;
			}
		}
		if(argc == 2)
			showError("'dir name' argument not specified\n"\
						   "Try 'mkdir -h' for more information\n");
	}
	for(int pos = (flagsexist == 0) ? 1 : 2; pos < argc; pos++){
		#if defined(__linux__)
			int status = mkdir(argv[pos], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		#else
			int status = mkdir(argv[pos]);
		#endif
		switch(status){
			case 0:
				printf("Created dir '%s'\n", argv[pos]);
				break;
			case EEXIST:
				if(!fparents)
					showError("Directory is exist\n");
				break;
			default:
				if(access(argv[pos], F_OK)){
					char path[1024];
					memset(path, 0, sizeof(path));
					char *dir = strtok(argv[pos], "/");
					while(dir != NULL){
						strcat(path, dir);
						strcat(path, "/");
						printf("Created dir '%s'\n", path);
						#if defined(__linux__)
							mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
						#else
							mkdir(path);
						#endif
						
						dir = strtok(NULL, "/");
					}
					
				} else
					perror("mkdir");
				break;			
		}
	}
}
