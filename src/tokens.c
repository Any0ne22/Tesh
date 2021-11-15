#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




token* new_token(){
    token* TheToken = malloc(sizeof(token));
    TheToken->element=calloc(1,sizeof(char*));
    TheToken->size=0;
    return TheToken;
}


token* parse(char* input){
    token* TheToken= new_token();
    const char* limitations= " ";
    char* case_t = strtok(input,limitations);

    while(case_t!=NULL){
        printf("%s\n",case_t);
        TheToken->element[TheToken->size] = strdup(case_t);
        TheToken->size+=1;
        TheToken->element= realloc(TheToken->element,(TheToken->size+1)*sizeof(char*));
        case_t = strtok(NULL,limitations);
    }

    TheToken->element[TheToken->size] = NULL;
    return TheToken;
}

void add_token(token* theToken, char* input){
    theToken->element= realloc(theToken->element,(theToken->size+2)*sizeof(char*));
    theToken->element[theToken->size] = strdup(input);
    theToken->element[theToken->size+1] = NULL;
    theToken->size+=1;
}


void destroy_token(token* theToken){
/* fonction qui libère l'espace mémoire alloué à une structure token
Retourne: rien*/

    for(int i=0;i<theToken->size;i++){
        free(theToken->element[i]);
    }
    free(theToken->element);
    free(theToken);
}


void clear_token(token* theToken){
    for(int i=0;i<theToken->size;i++){
        free(theToken->element[i]);
    }
    theToken->size=0;
    theToken->element= realloc(theToken->element,sizeof(char*));
    theToken->element[0]=NULL;
}


