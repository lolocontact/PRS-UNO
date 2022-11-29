#include "libs.h"
/** **********************************
 * \file      my_tube.h
 * \author    Loïc PETITPREZ
 * \version   1.0
 * \date      29/09/22
 * \brief     Bibliothèque Personnel de gestion de tube
 * **********************************/

//droit utilisateur UNIX des tubes
const mode_t FIFO_MODE = 0760;
//0644

/************************************************************************************************/
/*Fonction : createTube										*/
/* Description : Créer un tube nommé avec les droits UNIX definie dans la variable FIFO_MODE	*/
/************************************************************************************************/
void createTube( char * name ){
	//Suppresion tube existant
	unlink(name);
	
	//création tube
	if (mkfifo(name, FIFO_MODE) == -1)
    	{
        	perror("Erreur de création du tube: %s\n");
        	exit(EXIT_FAILURE);
    	}
}

/************************************************************************************************/
/*Fonction : readTube										*/
/* Description : ouvre un tube en read only (côté client), lit dans le tube, puis le clot	*/
/************************************************************************************************/
int readTube( char * name, char * msg, int size){
	//entrée du tube
	int valread;
	//ouverture du tube en mode read only (côté client)
	if ((valread = open(name, O_RDONLY | O_NONBLOCK)) == -1)
	{
        	perror("Impossible d'ouvrir le tube en lecture:");
        	exit(EXIT_FAILURE);
	}
	//on lit le message dans le tube
	read(valread, msg, size);
	//on clot l'ouverture du tube (on ne le laisse pas en mode lecture)
	//close(valread);
	return valread;
}


/************************************************************************************************/
/*Fonction : writeTube										*/
/* Description : ouvre un tube en write only (côté serveur), écrit dedans, puis clot		*/
/************************************************************************************************/
int writeTube(char * name, char * msg, int size){
	//entrée du tube
	int valwrite;
	//ouverture du tube en mode write only (côté serveur).
	if ((valwrite = open(name, O_RDWR)) == -1) //O_RDWR O_WRONLY
    	{
        	perror("Impossible d'ouvrir le tube en écriture:");
        	exit(EXIT_FAILURE);
    	}
	//on écrit le message dans le tube
	write(valwrite, msg, size);
	//on clot l'ouverture du tube (on ne le laisse pas en mode écriture)
	//close(valwrite);
	return valwrite;
}

/************************************************************************************************/
/*Fonction : nameTube										*/
/* Description : prends un pid (int) et retourne un string sous la forme "/tmp/<pid>.fifo"	*/
/************************************************************************************************/
char * nameTube( int pid ){
	char* pidstr = malloc(100);
	char* nomTube = malloc(100);
	strcpy(nomTube, "/tmp/");
	sprintf(pidstr, "%d", pid);
	strcat(nomTube,pidstr); strcat(nomTube,".fifo");
	return nomTube;
}
