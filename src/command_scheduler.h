/** @file command_scheduler.h
 *  @brief Manage background and foreground processes
 *
 *  This file contains the function signatures for lauching command_runner
 *  functions in foreground or background and keep tracks of running processes.
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "process.h"
#include "command_runner.h"
#include "tokens.h"
#include "signals.h"

/** @brief Run a command in background or foreground
 * 
 *  Parse the command given as argument and run the command in background (if
 *  the command ends with &) or in foreground.
 *  If the command run in background, a new process is created to run the
 *  command and its pid is stored until the command ends. Else, the command
 *  run in foreground and the command return when the process ends.
 * 
 *  @param cmd an array of tokens with the command to run
 *  @param error set to true if you want to kill tesh on command status != 0
 */
void command_scheduler(tokens* cmd,bool error);

/** @brief Kill the command running in foreground
 * 
 *  Send SIGINT to the command running in foreground and all its child
 *  processes
 */
void kill_foreground();

/** @brief Put a background process in foreground
 * 
 *  Find the process with the given pid (or one of the background processes if
 *  pid is 0).
 * 
 *  @param pid the pid of a background process (or 0 for any)
 */
void put_in_foreground(pid_t pid);
