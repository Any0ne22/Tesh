#include "prompt.h"
#include <unistd.h>

char* prompt(){
    char* nom;
    int hostname =gethostname(nom,10);
    char* Repertoire= get_current_dir_name();
    char* prompt="%s@%s:%s$", Utilisateur, Hostname,Repertoire;
    return prompt;
}