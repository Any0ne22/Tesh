#include "process.h"

process* new_process() {
	process* p = calloc(1,sizeof(process));
	pipe(p->fd_out);
	p->status = -1;
	return p;
}

process* piped_process(process* p1) {
	process* p = calloc(1,sizeof(process));
	p->fd_in[0] = p1->fd_out[0];
	p->fd_in[1] = p1->fd_out[1];
	pipe(p->fd_out);
	p->status = -1;
	return p;
}

process* piped_process_from_file( char* filename) {
	process* p = calloc(1,sizeof(process));
	int file_fd = open(filename, O_RDONLY, S_IRUSR);
    dup2(file_fd, p->fd_in[1]);
	pipe(p->fd_out);
	p->status = -1;
	return p;
}

int launch_process(process* p, char* args[], bool pipeOutput) {
	if(args[0] == NULL) return -1;	//If there are no arguments
	int pid = fork();
	if(!pid) {
		// Child process
		if (pipeOutput) {
			close(p->fd_out[0]);
    		dup2(p->fd_out[1],STDOUT_FILENO);
			close(p->fd_out[1]);
		}

		if(p->fd_in[0] != 0 || p->fd_in[1] != 0) {
			// Pipe the standard input to the fd if it is initialized
			close(p->fd_in[1]);
    		dup2(p->fd_in[0],STDIN_FILENO);
			close(p->fd_in[0]);
		}

		execvp(args[0], args);
		if(errno == 2) {
			printf("Error: command %s not found\n", args[0]);
		}
		exit(errno);
	} else {
		// Parent process
		if(p->fd_in[0] != 0 || p->fd_in[1] != 0) {
			// Close the descriptors if there was a pipe on the standard input
			close(p->fd_in[0]);
			close(p->fd_in[1]);
		}
		p->pid = pid;
		return pid;
	}
}

int pipe_to_file(process* p, char* args[], char* filename, bool append) {
	if(args[0] == NULL) return -1;	//If there are no arguments
	int pid = fork();
	if(!pid) {
		// Connecting the output of the process to a file descriptor
		close(p->fd_out[0]);
		close(p->fd_out[1]);

		// Open the file with the user rights and create it if necessary
		int flags = (append ? O_APPEND : O_TRUNC) | O_WRONLY | O_CREAT;
		int file_fd = open(filename, flags, S_IRUSR | S_IWUSR);
    	dup2(file_fd,STDOUT_FILENO);

		if(p->fd_in[0] != 0 || p->fd_in[1] != 0) {
			// Pipe the standard input to the fd if it is initialized
			close(p->fd_in[1]);
    		dup2(p->fd_in[0],STDIN_FILENO);
			close(p->fd_in[0]);
		}
		int errcode = execvp(args[0], args);
		exit(errcode);
	} else {
		// Parent process
		if(p->fd_in[0] != 0 || p->fd_in[1] != 0) {
			// Close the descriptors if there was a pipe on the standard input
			close(p->fd_in[0]);
			close(p->fd_in[1]);
		}
		p->pid = pid;
		return pid;
	}
}

int launch_and_print(process* p, char* args[]) {
	return launch_process(p, args, false);
}

int launch_and_pipe(process* p, char* args[]) {
	return launch_process(p, args, true);
}

int wait_status(process* p) {
	waitpid(p->pid, &p->status, 0);
	return WEXITSTATUS(p->status);
}

void free_process(process* p) {
	free(p);
}
