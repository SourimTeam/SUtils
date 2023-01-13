#include <stdio.h>

int main(int argc, char **argv){
	for(int pos = 1; pos < argc; pos++)
		printf("%s ", argv[pos]);
	putchar('\n');
	return 0;
}