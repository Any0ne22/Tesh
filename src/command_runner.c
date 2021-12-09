#include "command_runner.h"
#include "command_scheduler.h"
#include <ctype.h>

pid_t command_runner(tokens* theToken,bool erreur){
    /* fonction qui va noter les ; et || et va rediriger dans un 
    nouveau tableau les commadnes suivantes et exécuter la première si nécessaire*/
    tokens* theCommand = new_tokens();
    process* p= new_process();
    bool isSkipped = false;
    bool isSuccessful = true;
    for(int i=0; i<theToken->size;i++){
        if(i==0 && strcmp(theToken->elements[i],"cd")==0){
            make_cd(theToken->elements[i+1]);
            i++;
        }
        if(i==0 && strcmp(theToken->elements[i],"fg")==0){
            if(theToken->size>=i+1 && isdigit(theToken->elements[i+1])){
                put_in_foreground(atoi(theToken->elements[i+1]));
            }
            else{
                put_in_foreground(0);
            }
            i++;
        }
        else if(strcmp(theToken->elements[i],";")==0){
            /* on exécute la commande d'avant et on continue à stocker 
                pour exécution les commandes d'après*/
            isSkipped =false;
            isSuccessful = true,
            launch_and_print(p, theCommand->elements);
            int status=wait_status(p);
            if(status!=0 && erreur){
                exit(0);
            }
            free_process(p);
            p = new_process();   
            clear_tokens(theCommand);
            continue;
        }
        else if(strcmp(theToken->elements[i],"&&")==0){
            /* dans ce cas, il faut exécuter la commande d'avant et celle d'après uniquement 
            si le code de retour est de 0 */
            if(isSkipped) {
                if(isSuccessful) isSkipped = false;
                continue;
            }
            launch_and_print(p,theCommand->elements);
            int retour= wait_status(p);
            free_process(p);
            p = new_process();
            clear_tokens(theCommand);      
            if(retour==0){
               isSuccessful = true;
               continue;
            }
            else{
                isSkipped = true;
                isSuccessful = false;
                if (erreur)exit(0);
            }
        }

        else if(strcmp(theToken->elements[i],"||")==0){
            /* il faut exécuter la commande d'avant et celle d'après uniquement si le code 
            de retour est différent de 0*/
            if(isSkipped) {
                if(!isSuccessful) isSkipped = false;
                continue;
            }
            launch_and_print(p,theCommand->elements);
            int retour= wait_status(p);
            free_process(p);
            p = new_process();
            clear_tokens(theCommand);
            if(retour!=0){
                if (erreur)exit(0);
                isSuccessful = false;
                continue;
            }
            else{
                isSkipped = true;
                isSuccessful = true;
            }
        } 
        else if(isSkipped==true){
            continue;
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
            int status=wait_status(p);
            clear_tokens(theCommand);
            free_process(p);
            if(status!=0 && erreur){
                exit(0);
            }
            p = new_process();
            isSkipped=true;
        }
        else if(strcmp(theToken->elements[i],">>")==0){
            pipe_to_file(p,theCommand->elements,theToken->elements[i+1],true);
            int status=wait_status(p);
            clear_tokens(theCommand);
            free_process(p);
            if(status!=0 && erreur){
                exit(0);
            }
            p = new_process();
            i++;
        }
        else if(strcmp(theToken->elements[i],"<")==0){
            free_process(p);
            p = piped_process_from_file(theToken->elements[i+1]);
            i++;
        }
        else{
            add_token(theCommand,theToken->elements[i]);
        }
    }              
    launch_and_print(p, theCommand->elements);
    int status=wait_status(p);
    pid_t pid = p->pid;
    free_process(p);
    destroy_tokens(theCommand);
    if(status!=0 && erreur){
        exit(0);
    }         
    return pid;
}

