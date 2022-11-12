#include "libs.h"
/** **********************************
 * \file      my_tube.h
 * \author    Loïc PETITPREZ
 * \version   1.0
 * \date      29/09/22
 * \brief     Bibliothèque Personnel de gestion de tube
 * **********************************/
 
 const mode_t FIFO_MODE = 0760;

/************************************************************************************************/
/*Fonction : createTube										*/
/* Description : créer un tube nommé name							*/
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
/*Fonction : connectTubeRead									*/
/* Description : se connecte au tube nommé <name> en lecture					*/
/************************************************************************************************/
int connectTubeRead( char * name ){
	int valread;
	if ((valread = open(name, O_RDONLY | O_NONBLOCK)) == -1)
	{
        	perror("Impossible d'ouvrir le tube en lecture:");
        	exit(EXIT_FAILURE);
	}
	return valread;
}

/************************************************************************************************/
/*Fonction : readTube										*/
/* Description : lis dans le tube								*/
/************************************************************************************************/
void readTube( int valread, char * buffer, int size /*MSIZE*/){
	read(valread, buffer, size);
}

/************************************************************************************************/
/*Fonction : connectTubeWrite									*/
/* Description : se connecte au tube nommé <name> en écriture					*/
/************************************************************************************************/
int connectTubeWrite( char * name ){
	int valwrite;
	if ((valwrite = open(name, O_WRONLY)) == -1)
    	{
        	perror("Impossible d'ouvrir le tube en écriture:");
        	exit(EXIT_FAILURE);
    	}
	return valwrite;
}

/************************************************************************************************/
/*Fonction : writeTube										*/
/* Description : écris dans le tube								*/
/************************************************************************************************/
void writeTube( int valwrite, char * buffer, int size /*MSIZE*/){
	write(valwrite, buffer, size);
}

/************************************************************************************************/
/*Fonction : nameTube										*/
/* Description : prends un pid et retourne un string sous la forme "/tmp/<pid>.fifo"		*/
/* Entrees : int pid										*/
/* Sorties : retourne string "/tmp/<pid>.fifo"							*/
/************************************************************************************************/
char * nameTube( int pid ){
	char* pidstr = malloc(100);
	char* nomTube = malloc(100);
	strcpy(nomTube, "/tmp/");
	sprintf(pidstr, "%d", pid);
	strcat(nomTube,pidstr); strcat(nomTube,".fifo");
	return nomTube;
}
