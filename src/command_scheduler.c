#include "command_scheduler.h"

/// The PID of the command running in foreground
pid_t foreground = 0;
/// A list of PIDs of processes running in background
pid_t* background = NULL;
/// The size of background process array
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
	// Checking state of background commands
	check_running_commands();
	// Searching for &
	if(strcmp(cmd->elements[cmd->size-1],"&") == 0) {
		// If there is & at the end of the command, run it in background
		
		// Removing & form the token list to execute the command
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
		// Else, run the command in foreground
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
