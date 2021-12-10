/** @file utils.h
 *  @brief Some useful functions
 *
 *  This file contains the function signatures of many utility functions
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define STDOUT_BUFFER_SIZE 4096

/** @brief Check if a char* is a number
 * 
 *  @param str the string to check
 *  @return true if the string is a number
 */
bool isnumber(char* str);

/** @brief Read a line from a file descriptor or NULL if the fd is empty
 * 
 *  @param fd the file descriptor from which the content is read
 *  @return a char* containing a line from the file descriptor
 */
char* readLineFrom(int fd);

/** @brief Print a line to stdout
 * 
 *  This function cut the char* given in chunks of 32 caracters and write them to stdout
 *  to prevent the line to be cutted.
 * 
 *  @param line the line to print
 */
void printToStdout(char* line);