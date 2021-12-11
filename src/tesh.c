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
#include "utils.h"


int main(int argc, char *argv[])
{
	// Increasing the stdout buffer
	static char stdoutbuf[STDOUT_BUFFER_SIZE];
	setvbuf(stdout, stdoutbuf, _IOFBF, sizeof(stdoutbuf));
	// Loading libreadline with dlopen
	char* (*readline)(char*);
	void* (*add_history)(char*);
	void* handle = dlopen("libreadline.so", RTLD_NOW);
	*(void **) (&readline) = dlsym(handle, "readline");
	*(void **) (&add_history) = dlsym(handle, "add_history");

	// Parsing args
	parametres* param = read_param(argc,argv);
	bool interactive = false;
	if (isatty(STDIN_FILENO)) interactive = true;

	int source = STDIN_FILENO;
	if(param->fichier != NULL) {
		source = open(param->fichier, O_RDONLY, S_IRUSR);
		if(errno == ENOENT){
			printf("File not found %s\n", param->fichier);
			exit(EXIT_FAILURE);
		}
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
		command_scheduler(tokens, param->erreur);
		destroy_tokens(tokens);
	}
	destroy_param(param);
	if(!interactive) close(source);
    return EXIT_SUCCESS;
}