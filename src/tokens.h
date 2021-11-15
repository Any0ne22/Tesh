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
    char** element;
    int size;
}token;


token* parse(char* input);
void destroy_token(token * theToken);
token* new_token();
void add_token(token* theToken, char* input);
void clear_token(token* theToken);

