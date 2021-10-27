#include <stdlib.h>
#include <stdio.h>
#include "snow/snow.h"
#include "process.h"

describe(process) {
    it("creating and freeing process") {
        process* p = new_process();
        assert(p->fd_in[0] == 0 && p->fd_in[1] == 0);
        assert(p->fd_out[0] != 0 || p->fd_out[1] != 0);
        assert(p->pid == 0);
        assert(p->status == -1);
        free_process(p);
    }

    it("creating and freeing piped process") {
        process* p = new_process();
        process* piped = piped_process(p);
        assert(piped->fd_in[0] == p->fd_out[0] && piped->fd_in[1] == p->fd_out[1]);
        assert(p->fd_out[0] != 0 || p->fd_out[1] != 0);
        assert(p->pid == 0);
        assert(p->status == -1);
        free_process(p);
        free_process(piped);
    }

    it("lauch and wait") {
        process* p = new_process();
        char* command1[5] = {"test", "1", "-eq", "1", NULL};
        launch_and_print(p, command1);
        int status1 = wait_status(p);
        assert(status1 == 0);

        char* command2[5] = {"test", "1", "-eq", "2", NULL};
        launch_and_print(p, command2);
        int status2 = wait_status(p);
        assert(status2 != 0);

        free_process(p);
    }

    it("lauch and pipe") {
        process* p = new_process();
        process* piped = piped_process(p);
        char* command1[3] = {"echo", "some text", NULL};
        launch_and_pipe(p, command1);
        
        char* command2[3] = {"grep", ".", NULL};
        launch_and_pipe(piped, command2);
        int status2 = wait_status(piped);
        assert(status2 == 0);

        free_process(p);
        free_process(piped);
    }

    it("lauch and pipe 2") {
        process* p = new_process();
        process* piped = piped_process(p);
        char* command1[2] = {"echo", NULL};
        launch_and_pipe(p, command1);
        
        char* command2[3] = {"grep", ".", NULL};
        launch_and_pipe(piped, command2);
        int status2 = wait_status(piped);
        assert(status2 != 0);

        free_process(p);
        free_process(piped);
    }
}

snow_main();
