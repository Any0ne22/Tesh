#include "command_scheduler.h"


#include <string.h>

/// The PID of the command running in foreground
pid_t foreground = 0;
/// A list of PIDs of processes running in background
pid_t* background = NULL;
/// The size of background process array
int backgroundProcessNumber = 0;


void command_scheduler(tokens* cmd, bool erreur) {
	// Searching for &
	if(strcmp(cmd->elements[cmd->size-1],"&") == 0) {
		// If there is & at the end of the command, run it in background
		
		// Removing & form the token list to execute the command
		free(cmd->elements[cmd->size-1]);
		cmd->elements[cmd->size-1] = '\0';
        cmd->size--;

		pid_t pid = fork();
		if(!pid) {
			char* msg = malloc(32*sizeof(char));
			snprintf(msg, 32, "[%d]\n", getpid());
			write(STDOUT_FILENO, msg, strlen(msg));
			fflush(stdout);
			free(msg);
			int status = command_runner(cmd, erreur);
			exit(status);
		} else {
			background = realloc(background, (backgroundProcessNumber+1)*sizeof(pid_t));
			background[backgroundProcessNumber++] = pid;
		}
	} else {
		command_runner(cmd,erreur);
	}
}

/// Not used for now
void kill_foreground() {
    if(foreground != 0) kill(-foreground, SIGINT);
}

pid_t get_background_pid() {
	if(backgroundProcessNumber == 0) return 0;
	pid_t pid = background[--backgroundProcessNumber];
	while(!pid && backgroundProcessNumber > 0) {
		pid = background[--backgroundProcessNumber];
		// If the process is not running, go to the next pid in the list
		if(waitpid(pid, NULL, WNOHANG) != 0) pid = 0;
	}
	pid_t* newBackground = malloc((backgroundProcessNumber)*sizeof(pid_t));
	for(int i = 0; i < backgroundProcessNumber; i++) {
		newBackground[i] = background[i];
	}
	free(background);
	background = newBackground;
	return pid;
}

bool is_valid_pid(pid_t pid) {
	for(int i = backgroundProcessNumber -1; i >= 0; i--) {
		if(background[i] == pid) return true;
	}
	return false;
}

void put_in_foreground(pid_t pid) {
	int status;
	if(!pid) pid = get_background_pid();
	else if(!is_valid_pid(pid)) return;
	if(!pid) return;
	foreground = pid;
	pid = waitpid(pid, &status, WUNTRACED);
	foreground = 0;
	char* msg = malloc(32*sizeof(char));
	snprintf(msg, 32,"[%d->%d]\n", pid, WEXITSTATUS(status));
	write(STDOUT_FILENO, msg, strlen(msg));
	free(msg);
}
