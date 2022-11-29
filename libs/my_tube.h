/** **********************************
 * \file      my_tube.h
 * \author    Loïc PETITPREZ
 * \version   1.0
 * \date      29/09/22
 * \brief     Bibliothèque Personnel de gestion de tube
 * **********************************/

//definitions :
/************************************************************************************************/
/*Fonction : createTube										*/
/* Description : Créer un tube nommé avec les droits UNIX definie dans la variable FIFO_MODE	*/
/************************************************************************************************/
void createTube( char * name );

/************************************************************************************************/
/*Fonction : readTube										*/
/* Description : ouvre un tube en read only (côté client), lit dans le tube, puis le clot	*/
/************************************************************************************************/
int readTube(char * name, char * msg, int size /*MSIZE*/);

/************************************************************************************************/
/*Fonction : writeTube										*/
/* Description : ouvre un tube en write only (côté serveur), écrit dedans, puis clot		*/
/************************************************************************************************/
int writeTube(char * name, char * msg, int size);

/************************************************************************************************/
/*Fonction : nameTube										*/
/* Description : prends un pid (int) et retourne un string sous la forme "/tmp/<pid>.fifo"	*/
/************************************************************************************************/
char * nameTube( int pid );

