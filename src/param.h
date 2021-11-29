/** @file param.h
 *  @brief Read parameters
 *
 *  This file contains the function signatures for reading the parameters 
 *  of the command line
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#include <stdlib.h>
#include <stdbool.h>
#pragma once

typedef struct parametres_ {
	bool erreur;
	bool readline;
	char* fichier;
} parametres;

/** @brief Create a new struct
 * 
 *  Create a new structrue to host the command line parameters
 *  @return a pointer to the new structure
 */
parametres* new_param();

/** @brief Read and store parameters
 * 
 *  Read the command line parameters and store their informations in a 
 *  new struct 
 *  @param p1 the number of arguments passed in the command line
 *  @param p2 a table of the arguments
 *  @return a pointer to the structure
 */
parametres* read_param(int argc, char* argv[]);

/** @brief Free the memory
 * 
 *  Free the memory occupied by a struct of parameters
 *  @return void
 */
void destroy_param(parametres* param);
