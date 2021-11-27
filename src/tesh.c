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

char* readLineFromStdin() {
	char* result = NULL;
	int size = 0;
	char c[1];
	while(read(STDIN_FILENO, c, 1) && c[0] != '\n' && c[0] != EOF) {
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
    bool interactive = false;
	if (isatty(fileno(stdin))) interactive = true;

	sig_setter();

	bool loop = true;

	while (loop) {
		char* input = NULL;
		
		if(interactive) {
			char* prompt = make_prompt();

			input = readline(prompt);
			free(prompt);
			add_history(input);
			if(input == NULL) {
				loop = false;
				if(interactive) printf("exit\n");
				break;
			}

		} else {
			if((input = readLineFromStdin()) == NULL){
				break;
			}
		}
		
		if(strlen(input) == 0) {
			free(input);
			continue;
		}
		if(!interactive) strtok(input, "\n");

		tokens* tokens = parse(input);
		command_scheduler(tokens);
		destroy_tokens(tokens);
		free(input);
	}


    return EXIT_SUCCESS;
}