/** @file tokens.h
 *  @brief Split a string into tokens
 *
 *  Split a string into an array of tokens.
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once

typedef struct _tokens{
    char** elements;
    int size;
}tokens;


tokens* parse(char* input);
void destroy_tokens(tokens * theToken);
tokens* new_tokens();
void add_token(tokens* theToken, char* input);
void clear_tokens(tokens* theToken);

