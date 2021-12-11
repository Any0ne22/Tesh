#include "utils.h"

bool isnumber(char* str) {
    for(int i=0; i < strlen(str); i++) if(!isdigit(str[i])) return false;
    return true;
}


char* readLineFrom(int fd) {
	char* result = NULL;
	int size = 0;
	char c[1];
	while(read(fd, c, 1) && c[0] != '\n' && c[0] != EOF) {
		result = realloc(result, (++size)*sizeof(char));
		result[size-1] = c[0];
	}
	if(size != 0 || c[0] == '\n') {
		result = realloc(result, (++size)*sizeof(char));
		result[size-1] = '\0';
	}
	return result;
}


void printToStdout(char* line) {
	for(int i=0; i < strlen(line); i+=STDOUT_BUFFER_SIZE) {
		write(STDOUT_FILENO, line+i, strlen(line)-i < STDOUT_BUFFER_SIZE ? strlen(line)-i : STDOUT_BUFFER_SIZE);
	}
}