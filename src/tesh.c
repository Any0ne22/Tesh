#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tokens.h"
#include "command_runner.h"


int main(int argc, char *argv[])
{
    bool interactive = false;
	if (isatty(fileno(stdin))) interactive = true;


	bool loop = true;

	/*while (loop) {
		char* prompt = NULL;
		if(interactive) prompt = "Prompt :";
		char* input = readline(prompt);
		
		add_history(input);

		if(input != NULL) {
			if(strlen(input) == 0) {
				free(input);
				continue;
			}
			tokens* tokens= parse(input);
			command_runner(tokens);

            free(input);
			destroy_tokens(tokens);
		} 
		else {
			// If stdin received an EOF (for example via CTRL+D)
			loop = false;
			if(interactive) printf("exit\n");
		}
	}*/

	while (loop) {
		char* input = NULL;
		size_t length = 0;
		
		if(interactive) {
			char* prompt = NULL;
			prompt = "Prompt :";
			input = readline(prompt);
			add_history(input);
			if(input == NULL) {
				loop = false;
				if(interactive) printf("exit\n");
				break;
			}
		} else {
			if(getline(&input, &length, stdin) == -1)  break;
		}
		
		if(strlen(input) == 0) {
			free(input);
			continue;
		}
		if(!interactive) strtok(input, "\n");

		tokens* tokens = parse(input);
		command_runner(tokens);

		free(input);
		destroy_tokens(tokens);
	}


    return EXIT_SUCCESS;
}