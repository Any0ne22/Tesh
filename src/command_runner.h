/** @file command_runner.h
 *  @brief Run commands from tokens
 *
 *  This file contains the function signatures for lauching commands given
 *  an array of tokens. These functions are responsible for interpreting
 *  the commands (finding the end of a command, interpreting &&, ||, ; , >,
 *  < , >>, <<, |, ...)
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once
#include "tokens.h"

/** @brief Execute the command
 * 
 *  Search the elements of the command and execute them in order 
 * 
 *  @param p the parsed command to execute
 *  @return void
 */
void command_runner(tokens* theToken);
