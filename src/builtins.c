#include "builtins.h"
#include <stdio.h>
#include <unistd.h>

void make_cd(char* filename){
    int resultat = chdir(filename);
}
