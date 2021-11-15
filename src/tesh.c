#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tokens.h"


int main(int argc, char *argv[])
{
    bool interactive = false;
	if (isatty(fileno(stdin))) interactive = true;


	bool loop = true;

	while (loop) {
		char* prompt = NULL;
		if(interactive) prompt = "Prompt :";
		char* input = readline(prompt);
		
		add_history(input);

		if(input != NULL) {
			if(strlen(input) == 0) {
				free(input);
				continue;
			}
			token* tokens= parse(input);
			add_token(tokens,"cat");

			if(!fork()){
				execvp(tokens->element[0],tokens->element);
				exit(0);
			}
            free(input);
			clear_token(tokens);
			destroy_token(tokens);
		} 
		else {
			// If stdin received an EOF (for example via CTRL+D)
			loop = false;
			if(interactive) printf("exit\n");
		}
	}

    return EXIT_SUCCESS;
}