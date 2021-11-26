#include "command_scheduler.h"
#include <errno.h>

pid_t foreground = 0;
pid_t* background = NULL;
int backgroundProcessNumber = 0;


void check_running_commands() {
	bool clean = true;
    for(int i=0; i < backgroundProcessNumber; i++) {
		if(!background[i]) continue;
		clean = false;
        kill(background[i], 0);
		if(errno != 0) {
			background[i] = 0;
		}
    }
	if(clean) {
		background = realloc(background, 0);
		backgroundProcessNumber = 0;
	}
}

void command_scheduler(tokens* cmd) {
	check_running_commands();
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
			printf("[%d, %d]\n", backgroundProcessNumber, pid);
		}
	} else {
		pid_t pid = fork();
		if(!pid) {
			sig_setter_process();
			command_runner(cmd);
			exit(0);
		} else {
			foreground = pid;
			waitpid(pid, NULL, 0);
			foreground = 0;
		}
	}
}

void kill_foreground() {
    if(foreground != 0) kill(-foreground, SIGINT);
}
