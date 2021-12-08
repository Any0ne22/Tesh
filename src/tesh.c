#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <dlfcn.h>
#include "tokens.h"
#include "command_runner.h"
#include "command_scheduler.h"
#include "signals.h"
#include "prompt.h"
#include "param.h"
#define STDOUT_BUFFER_SIZE 4096


/** @brief Read a line from a file descriptor or NULL if the fd is empty
 * 
 *  @param fd the file descriptor from which the content is read
 *  @return a char* containing a line from the file descriptor
 */
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

/** @brief Print a line to stdout
 * 
 *  This function cut the char* given in chunks of 32 caracters and write them to stdout
 *  to prevent the line to be cutted.
 * 
 *  @param line the line to print
 */
void printToStdout(char* line) {
	for(int i=0; i < strlen(line); i+=STDOUT_BUFFER_SIZE) {
		write(STDOUT_FILENO, line+i, strlen(line)-i < STDOUT_BUFFER_SIZE ? strlen(line)-i : STDOUT_BUFFER_SIZE);
	}
}

int main(int argc, char *argv[])
{
	// Increasing the stdout buffer
	static char stdoutbuf[STDOUT_BUFFER_SIZE];
	setvbuf(stdout, stdoutbuf, _IOFBF, sizeof(stdoutbuf));
	// Loading libreadline with dlopen
	char* (*readline)(char*);
	void* (*add_history)(char*);
	void* handle;
	handle = dlopen("libreadline.so", RTLD_NOW);
	*(void **) (&readline) = dlsym(handle, "readline");
	*(void **) (&add_history) = dlsym(handle, "add_history");

	parametres* param = read_param(argc,argv);
	bool interactive = false;
	if (isatty(STDIN_FILENO)) interactive = true;

	// Parsing args
	int source = STDIN_FILENO;
	if(param->fichier != NULL) {
		source = open(param->fichier, O_RDONLY, S_IRUSR);
		interactive = false;
	}

	sig_setter();

	bool loop = true;

	while (loop) {
		char* input = NULL;

		if(param->readline) {
			char* prompt = make_prompt();
			input = (*readline)(prompt);
			free(prompt);
			(*add_history)(input);
		} else if (interactive) {
			char* prompt = make_prompt();
			printToStdout(prompt);
			free(prompt);
			input = readLineFrom(source);
		} else {
			input = readLineFrom(source);
		}

		if(input == NULL) {
			break;
		}

		if(input[0] == '\0') {
			free(input);
			continue;
		}


		tokens* tokens = parse(input);
		free(input);
		if(param->erreur)command_scheduler(tokens, true);
		else command_scheduler(tokens,false);
		destroy_tokens(tokens);
	}
	destroy_param(param);
	if(!interactive) close(source);
    return EXIT_SUCCESS;
}