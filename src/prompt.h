/** @file prompt.h
 *  @brief Generate prompt
 *
 *  Generate a prompt containing username, hostname and working directory.
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/** @brief create prompt
 * 
 *  Search for the information to display on the prompt and assemble them in a string
 * 
 *  @return a string to display
 */
char* make_prompt();
