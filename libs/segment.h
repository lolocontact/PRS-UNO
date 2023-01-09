//definitions :

/************************************************************************************************/
/*Fonction : connecteSegment									*/
/* Description : Même algo que creerSegment, mais permet de distinguer la création et la	*/
/*		connection à un segment								*/
/************************************************************************************************/
int connecteSegment(int size, char *name, int cle /*1,2*/);

/************************************************************************************************/
/*Fonction : creerSegment									*/
/* Description : Créer un segment de mémoire partagée, retourne l'id du segment			*/
/************************************************************************************************/
int creerSegment(int size, char *name, int cle /*1,2*/);

/************************************************************************************************/
/*Fonction : supprimerSegment									*/
/* Description : Supprimer un segment de mémoire partagée					*/
/************************************************************************************************/
void supprimerSegment(int shmid);

/************************************************************************************************/
/*Fonction : detacherSegment									*/
/* Description : on délie le segment de mémoire partagée à la variable segmentADDRESS		*/
/************************************************************************************************/
void detacherSegment(char * segmentADDRESS);

/**************************************************************************************************/
/*Fonction : ecritureSegment                                                                      */
/* Description : On écrit sur un segment			                                  */
/**************************************************************************************************/
void ecritureSegment(int segmentID, int message);

/**************************************************************************************************/
/*Fonction : lireSegment                                                                          */
/* Description : Lis un segment					                                  */
/**************************************************************************************************/
int lireSegment(int shmId);

/************************************************************************************************/
/*Fonction : attacheSegment									*/
/* Description : Ici on fait un segment partagée de type char* attacheSegment			*/
/************************************************************************************************/
char * attacheSegment(int segmentID);


/************************************************************************************************/
/*Fonction : infoSegment									*/
/* Description : Donne des informations sur le segment de mémoire partagée			*/
/************************************************************************************************/
struct shmid_ds infoSegment(int shmid);


/* ==================================================== */
/* OPTIMISÉ                                             */
/* ==================================================== */

//après un attacheSegment(), garder la chaine où l'on écrira les messages
//on évite alors les shmat à répétitions

void ecritureSegment2(char * zoneEcriture, int message);

//inutile : mais pour comprendre le fonctionnement :
char * lireSegment2(char * message);

char * attacheSegment2(int segmentID, char * segmentADDRESS);


/* ==================================================== */
/* AUTRE                                             */
/* ==================================================== */

/**************************************************************************************************/
/*Fonction : envoieCarte	                                                                  */
/* Description : envoie 7 cartes	 			                                  */
/**************************************************************************************************/
void envoieCarte(int segmentID, carte message[carteEnDebutDePartie]);

/**************************************************************************************************/
/*Fonction : lireCarte  	 	                                                          */
/* Description : Reçois ses 7 cartes				                                  */
/**************************************************************************************************/
carte * lireCarte(int shmId);

/**************************************************************************************************/
/*Fonction : pid2string	                                                                          */
/* Description : créer un nom/emplacement de segment au nom du pid                                */
/**************************************************************************************************/
char * pid2string( int pid );

/**************************************************************************************************/
/*Fonction : ecriturePartie                                                                       */
/* Description : On écrit sur p					                                  */
/**************************************************************************************************/
//void ecriturePartie(int segmentID, struct partie message);

/**************************************************************************************************/
/*Fonction : lecturePartie                                                                        */
/* Description : Lis partie p					                                  */
/**************************************************************************************************/
//partie lecturePartie(int shmId);
