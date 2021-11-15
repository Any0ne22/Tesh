#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

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

            printf("%s\n", input);
            free(input);
		} else {
			// If stdin received an EOF (for example via CTRL+D)
			loop = false;
			if(interactive) printf("exit\n");
		}
	}

    return EXIT_SUCCESS;
}