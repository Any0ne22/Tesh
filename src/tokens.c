#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




tokens* new_tokens(){
    tokens* TheToken = malloc(sizeof(tokens));
    TheToken->elements=calloc(1,sizeof(char*));
    TheToken->size=0;
    return TheToken;
}


tokens* parse(char* input){
    tokens* TheToken= new_tokens();
    const char* limitations= " ";
    char* case_t = strtok(input,limitations);

    while(case_t!=NULL){
        char* copy=strdup(case_t);
        TheToken->elements[TheToken->size] = copy;
        TheToken->size+=1;
        TheToken->elements= realloc(TheToken->elements,(TheToken->size+1)*sizeof(char*));
        case_t = strtok(NULL,limitations);
    }
    TheToken->elements[TheToken->size] = NULL;
    return TheToken;
}

void add_token(tokens* theToken, char* input){
    if(theToken==NULL){
        return;
    }
    theToken->elements= realloc(theToken->elements,(theToken->size+2)*sizeof(char*));
    char* copy=strdup(input);
    theToken->elements[theToken->size] = copy;
    theToken->elements[theToken->size+1] = NULL;
    theToken->size+=1;
}


void destroy_tokens(tokens* theToken){
/* fonction qui libère l'espace mémoire alloué à une structure token
Retourne: rien*/

    for(int i=0;i<theToken->size;i++){
        free(theToken->elements[i]);
    }
    free(theToken->elements);
    free(theToken);
}


void clear_tokens(tokens* theToken){
    /* fonction qui enlève tous les tokens et laisse une structure tokens vide*/
    for(int i=0;i<theToken->size;i++){
        free(theToken->elements[i]);
    }
    theToken->size=0;
    theToken->elements= realloc(theToken->elements,sizeof(char*));
    theToken->elements[0]=NULL;
}


