#include "libs.h"

int connecteSegment(int size, char *name, int cle /*1,2*/){
	key_t cleSegment = ftok(name, cle); /* Clef du segment de mémoire partagée */
	if (cleSegment == - 1)
	{
		//printf("CLIENT - Erreur de création de clef %s\n",strerror(errno));
	}
	int shmId = shmget(cleSegment, size, 0666 );
	if (shmId == -1)
	{
		printf("CLIENT - Erreur de création du segment de mémoire partagée %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	return shmId ;
}

//shmId = shmget(cleSegment, 100 * sizeof(char),IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
//ftok(nomSegment, 1);

/**************************************************************************************************/
/*Fonction : creerSegment                                                                         */
/* Description : Créer un segment/une memoire partagée		                                  */
/**************************************************************************************************/
int creerSegment(int size, char *name, int cle /*1,2*/)
{
	// On récupère l'ID de notre segment de mémoire partagée
	key_t clef = ftok(name,(key_t) cle) ;
	int shmid = shmget( clef,size, IPC_CREAT | SHM_R | SHM_W) ;
  
	if ( shmid== -1 ) {
		printf("Erreur de création%s\n",strerror(errno)) ;
		exit(EXIT_FAILURE) ;  /*on sort du programme */
	}
	return shmid ;
}

/**************************************************************************************************/
/*Fonction : supprimerSegment                                                                     */
/* Description : Supprime un segment par son id			                                  */
/**************************************************************************************************/
void supprimerSegment(int shmid){
	if (shmctl(shmid,IPC_RMID,NULL) == -1){
		printf("Erreur de supression%s\n",strerror(errno)) ;
		exit(EXIT_FAILURE) ;
	}
}

/**************************************************************************************************/
/*Fonction : detacherSegment                                                                      */
/* Description : On se détache d'un segment			                                  */
/**************************************************************************************************/
void detacherSegment(char * segmentADDRESS){
	if( shmdt(segmentADDRESS) == -1){
		printf("Erreur de supression%s\n",strerror(errno)) ;
		exit(EXIT_FAILURE) ;  /*on sort du programme */
	}
}

/**************************************************************************************************/
/*Fonction : ecritureSegment                                                                      */
/* Description : On écrit sur un segment			                                  */
/**************************************************************************************************/
void ecritureSegment(int segmentID, char* message){
	char * zoneEcriture = shmat(segmentID,NULL,0);
	if (zoneEcriture == (char *)-1)
	{
		printf("SERVEUR - Impossible de s'attacher au segment de mémoire %d \n",segmentID);
		exit(EXIT_FAILURE);
	}
	sprintf(zoneEcriture,"%s",message);
}

/**************************************************************************************************/
/*Fonction : lireSegment                                                                          */
/* Description : Lis un segment					                                  */
/**************************************************************************************************/
char * lireSegment(int shmId){
	char * message = shmat(shmId,NULL,0);
	if (message == (char *)-1)
	{
		printf("CLIENT - Impossible de s'attacher au segment de mémoire : %d \n",shmId);
		exit(EXIT_FAILURE);
	}
	return message;
}

/**************************************************************************************************/
/*Fonction : attacheSegment                                                                       */
/* Description : S'attache a un segment				                                  */
/**************************************************************************************************/
char * attacheSegment(int segmentID){
	char * segmentADDRESS = shmat(segmentID, "NULL", 0);
	if (!segmentADDRESS)
	{
		printf("CLIENT - Erreur de création de clef %s\n",strerror(errno));
		exit(EXIT_FAILURE) ;  /*on sort du programme */
	}
	return segmentADDRESS;
}



/* ==================================================== */
/* OPTIMISÉ                                             */
/* ==================================================== */

//après un attacheSegment(), garder la chaine où l'on écrira les messages
//on évite alors les shmat à répétitions

void ecritureSegment2(char * zoneEcriture, int message){
	//char * zoneEcriture = shmat(segmentID,NULL,0);
	sprintf(zoneEcriture,"%d",message);
}

//inutile : mais pour comprendre le fonctionnement :
char * lireSegment2(char * message){
	//char * message = shmat(shmId,NULL,0);
	return message;
}

char * attacheSegment2(int segmentID, char * segmentADDRESS){
	segmentADDRESS = shmat(segmentID, "NULL", 0);
	if (!segmentADDRESS)
	{
		printf("CLIENT - Erreur de création de clef %s\n",strerror(errno));
		exit(EXIT_FAILURE) ;  /*on sort du programme */
	}
	printf("%s", segmentADDRESS);
	return segmentADDRESS;
}

/* ==================================================== */
/* AUTRE                                             */
/* ==================================================== */

/**************************************************************************************************/
/*Fonction : pid2string	                                                                          */
/* Description : créer un nom/emplacement de segment au nom du pid                                */
/**************************************************************************************************/
char * pid2string( int pid ){
	char* pidstr = malloc(100);
	char* nomTube = malloc(100);
	strcpy(nomTube, "/tmp/");
	sprintf(pidstr, "%d", pid);
	strcat(nomTube,pidstr); strcat(nomTube,"");
	return nomTube;
}
