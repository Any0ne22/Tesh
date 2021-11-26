#include "command_scheduler.h"

pid_t foreground = 0;
pid_t* background = NULL;
int backgroundProcessNumber = 0;

void command_scheduler(tokens* cmd) {
	// Consume cmd
	// Searching for &
	if(strcmp(cmd->elements[cmd->size-1],"&") == 0) {
		// If there is & at the end of the command, run it in background
		
		// On retire & de la liste de tokens pour pouvoir interpreter la commande
		free(cmd->elements[cmd->size-1]);
		cmd->elements[cmd->size-1] = '\0';
        cmd->size--;

		pid_t pid = fork();
		if(!pid) {
			command_runner(cmd);
			exit(0);
		} else {
			background = realloc(background, (backgroundProcessNumber+1)*sizeof(pid_t));
			background[backgroundProcessNumber++] = pid;
			printf("[%d]\n", pid);
		}
	} else {
		command_runner(cmd);
	}
}

void check_running_commands() {
    for(int i=0; i < backgroundProcessNumber; i++) {
        int status = 0;
    }
}

void kill_foreground() {
    if(foreground != 0) kill(-foreground, SIGINT);
}