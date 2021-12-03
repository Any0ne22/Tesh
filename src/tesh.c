#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tokens.h"
#include "command_runner.h"
#include "command_scheduler.h"
#include "signals.h"
#include "prompt.h"
#include "param.h"



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


int main(int argc, char *argv[])
{
	parametres* param = read_param(argc,argv);
	bool interactive = false;
	if (isatty(fileno(stdin))) interactive = true;

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
		
		if(interactive) {
			char* prompt = make_prompt();
			input = readline(prompt);
			free(prompt);
			add_history(input);
		} else {
			input = readLineFrom(source);
		}

		if(input == NULL) {
			if(interactive) printf("exit\n");
			break;
		}

		if(input[0] == '\0') {
			free(input);
			continue;
		}

		tokens* tokens = parse(input);
		free(input);
		command_scheduler(tokens);
		destroy_tokens(tokens);
	}

	close(source);
    return EXIT_SUCCESS;
}