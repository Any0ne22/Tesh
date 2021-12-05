#define _GNU_SOURCE
#include "prompt.h"

char* make_prompt(){
    char Hostname[255];
    char Utilisateur[255];
    char Repertoire[255];
    getcwd(Repertoire, 255);
    gethostname(Hostname,255);
    cuserid(Utilisateur);

    char* prompt=malloc(sizeof(char)*((strlen(Repertoire)+strlen(Utilisateur)+strlen(Hostname))+5));
    snprintf(prompt,1024, "%s@%s:%s$ ", Utilisateur, Hostname,Repertoire);
    return prompt;
}

