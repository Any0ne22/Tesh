/** @file signals.h
 *  @brief Manage signals
 *
 *  This file contains the function signatures for changing the signals handlers
 *  and manage signals
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "command_scheduler.h"

/** @brief Set the signal handler for the main process
 * 
 *  Set a signal handler that forward SIGINT to the running command
 */
void sig_setter();

/** @brief Set the signal handler for the the running command
 * 
 *  Set a signal handler that kills the process when SIGINT is received
 */
void sig_setter_process();