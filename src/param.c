#include "param.h"

parametres* new_param(){
	parametres* param = malloc(sizeof(parametres));
	param->erreur=false;
	param->readline=false;
	param->fichier=NULL;
	return param;
}

parametres* read_param(int argc, char* argv[]){
	parametres* param= new_param();
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"-e")==0){
			param->erreur=true;
		}
		else if(strcmp(argv[i],"-r")==0){
			param->readline=true;
		}
		else{
			param->fichier=strdup(argv[i]);
		}
	}
	return param;
}

void destroy_param(parametres* param){
    free(param->fichier);
    free(param);
}
