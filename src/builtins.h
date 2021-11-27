/** @file builtins.h
 *  @brief Tesh internal commands
 *
 *  This file contains the function signatures for Tesh built-ins
 *  commands such as cd or fg
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once

/** @brief Change directory
 * 
 *  Execute the command cd as a builtin command
 * 
 *  @param p1 the directory filename
 *  @return void
 */
void make_cd(char* filename);
