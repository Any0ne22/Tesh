#include "command_runner.h"
#include "tokens.h"



void command_runner(tokens* theToken){
    /* fonction qui va noter les ; et || et va rediriger dans un 
    nouveau tableau les commadnes suivantes et exécuter la première si nécessaire*/
    tokens* theCommand = new_token();
    for(int i=0; i<theToken->size;i++){
        if(theToken->elements[i]==";" || theToken->elements[i]=='||'){
            int retour=execvp(theCommand->elements[0],theCommand->elements[1]);
            destroy_tokens(theCommand);
            return;
        }
        if(theToken->elements[i]== '&&'){
            /* dans ce cas, il faut exécuter la commande et récupérer son retour pour éxécuter la commande d'après*/
            int retour=execvp(theCommand->elements[0],theCommand->elements[1]);
            clear_tokens(theCommand);
            return;
        }
        else{
            add_token(theCommand,theToken->elements[i]);
        }
    }
}

