#define _GNU_SOURCE
#include "prompt.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* make_prompt(){
    char Hostname[32];
    gethostname(&Hostname[0],32);
    char* Repertoire= get_current_dir_name();
    char* Utilisateur = getlogin();
    char* prompt=malloc(sizeof(char)*(strlen(Repertoire)+strlen(Utilisateur)+strlen(&Hostname[0])+5));
    sprintf(prompt,"%s@%s:%s$", Utilisateur, &Hostname[0],Repertoire);
    free(Repertoire);
    return prompt;
}

