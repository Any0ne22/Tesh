#include "command_runner.h"
#include "tokens.h"
#include "process.h"
#include <string.h>

void command_runner(tokens* theToken){
    /* fonction qui va noter les ; et || et va rediriger dans un 
    nouveau tableau les commadnes suivantes et exécuter la première si nécessaire*/
    tokens* theCommand = new_tokens();
    for(int i=0; i<theToken->size;i++){
        if(strcmp(theToken->elements[i],";")==0){
            /* on exécute la commande d'avant et on continue à stocker 
                pour exécution les commandes d'après*/
            process* p= new_process();
            launch_and_print(p, theCommand->elements);
            wait_status(p);
            clear_tokens(theCommand);
            free(p);
            continue;
        }
        else if(strcmp(theToken->elements[i],"&&")==0){
            /* dans ce cas, il faut exécuter la commande d'avant et celle d'après uniquement 
            si le code de retour est de 0 */
            process* p = new_process();
            launch_and_print(p,theCommand->elements);
            int retour= wait_status(p);
            free_process(p);
            if(retour==0){
               clear_tokens(theCommand);
               continue;
            }
            else{
                destroy_tokens(theCommand);
                return;
            }
        }
        else if(strcmp(theToken->elements[i],"||")==0){
            /* il faut exécuter la commande d'avant et celle d'après uniquement si le code 
            de retour est différent de 0*/
            process* p= new_process();
            launch_and_print(p,theCommand->elements);
            int retour= wait_status(p);
            free_process(p);
            if(retour!=0){
                clear_tokens(theCommand);
                continue;
            }
            else{
                destroy_tokens(theCommand);
                return;
            }
        } 
        else{
            add_token(theCommand,theToken->elements[i]);
        }
    }
    process* p= new_process();                
    launch_and_print(p, theCommand->elements);
    wait_status(p);                                
    free_process(p);
    destroy_tokens(theCommand);         
    return;
}

