/** @file tokens.h
 *  @brief Split a string into tokens
 *
 *  Split a string into an array of tokens.
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @brief A struct for managing tokens
 * 
 * Allows to store tokens and get the number of tokens in the structure
 *  
 */
typedef struct _tokens{
    char** elements;
    int size;
}tokens;

/** @brief Command parsing
 * 
 *  Separate each word of a command and store them in a struct
 * 
 *  @param p1 the command to parse
 *  @return a pointer to the tokens that contain the parsed command
 */
tokens* parse(char* input);

/** @brief Destroy toeksn struct
 * 
 *  Clear the struct of all the tokens and then destroy the structure
 *  
 *  @param p1 the tokens
 *  @return void
 */
void destroy_tokens(tokens * theToken);

/** @brief New tokens structure
 * 
 *  Create a new empty tokens struct
 * 
 *  @return a pointer to a tokens
 */
tokens* new_tokens();

/** @brief Add a token to a structure
 * 
 *  Add a token at the end of a tokens structure 
 * 
 *  @param p1 the tokens 
 *  @param p2 the token to add
 *  @return void
 */
void add_token(tokens* theToken, char* input);

/** @brief Free the tokens elements
 * 
 *  Clear the tokens from all the parsed elements, which leaves an empty structure
 * 
 *  @param p1 the tokens
 *  @return void
 */
void clear_tokens(tokens* theToken);

