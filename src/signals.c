#define _XOPEN_SOURCE 700
#include "signals.h"

void sig_handler(int sig) {
    if(sig == SIGINT) {
		// Send the SIGINT signal to the foreground process and all child processes
        kill_foreground();
		printf("\n");
	}
}

void sig_setter() {
    struct sigaction nvt,old;
	memset(&nvt, 0, sizeof(nvt));
	nvt.sa_handler = &sig_handler;
	sigaction(SIGINT, &nvt, &old);

	// Ignore SIGCHLD to allow child processes to die
	//signal(SIGCHLD, SIG_IGN);
}

void die(int sig) {
    exit(0);
}

void sig_setter_process() {
    struct sigaction nvt,old;
	memset(&nvt, 0, sizeof(nvt));
	nvt.sa_handler = &die;
	sigaction(SIGINT, &nvt, &old);
}
