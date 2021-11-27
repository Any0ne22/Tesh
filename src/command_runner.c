#include "command_runner.h"
#include "tokens.h"
#include "process.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include "stdbool.h"

void command_runner(tokens* theToken){
    /* fonction qui va noter les ; et || et va rediriger dans un 
    nouveau tableau les commadnes suivantes et exécuter la première si nécessaire*/
    tokens* theCommand = new_tokens();
    process* p= new_process();
    bool isSkipped = false;
    for(int i=0; i<theToken->size;i++){
        if(strcmp(theToken->elements[i],";")==0){
            /* on exécute la commande d'avant et on continue à stocker 
                pour exécution les commandes d'après*/
            isSkipped =false;
            launch_and_print(p, theCommand->elements);
            wait_status(p);
            free_process(p);
            p = new_process();   
            clear_tokens(theCommand);
            continue;
        }
        else if(isSkipped==true){
            continue;
        }
        else if(strcmp(theToken->elements[i],"&&")==0){
            /* dans ce cas, il faut exécuter la commande d'avant et celle d'après uniquement 
            si le code de retour est de 0 */
            launch_and_print(p,theCommand->elements);
            int retour= wait_status(p);
            free_process(p);
            p = new_process();             
            if(retour==0){
               clear_tokens(theCommand);
               continue;
            }
            else{
                isSkipped=true;
                clear_tokens(theCommand);
            }
        }
        /* rajouter si on l'execute après un point virgule quand meme avec un booléen*/

        else if(strcmp(theToken->elements[i],"||")==0){
            /* il faut exécuter la commande d'avant et celle d'après uniquement si le code 
            de retour est différent de 0*/
            launch_and_print(p,theCommand->elements);
            int retour= wait_status(p);
            free_process(p);
            p = new_process();
            if(retour!=0){
                clear_tokens(theCommand);
                continue;
            }
            else{
                isSkipped=true;
                clear_tokens(theCommand);
            }
        } 
        else if(strcmp(theToken->elements[i],"|")==0){
            launch_and_pipe(p,theCommand->elements);
            process* youhou = piped_process(p);
            clear_tokens(theCommand);
            free_process(p);
            p = youhou;
        }
        else if(strcmp(theToken->elements[i],">")==0){
            pipe_to_file(p,theCommand->elements,theToken->elements[i+1],false);
            clear_tokens(theCommand);
            free_process(p);
            p = new_process();
            isSkipped=true;
        }
        else if(strcmp(theToken->elements[i],">>")==0){
            pipe_to_file(p,theCommand->elements,theToken->elements[i+1],true);
            clear_tokens(theCommand);
            free_process(p);
            p = new_process();
            isSkipped=true;
        }
        else if(strcmp(theToken->elements[i],"<")==0){

        }
        else{
            add_token(theCommand,theToken->elements[i]);
        }
    }              
    launch_and_print(p, theCommand->elements);
    wait_status(p);                                
    free_process(p);
    destroy_tokens(theCommand);         
    return;
}

