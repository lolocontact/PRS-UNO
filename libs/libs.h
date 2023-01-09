#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>  
#include <errno.h>
#include <signal.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <time.h>

#include <semaphore.h>


//#include <semaphore.h>

#define SIZE 1000
#define MSIZE 64

#define CLE "uno.c"

#define MaxPseudo 20
#define MaxJoueurs 10
#define NbCartes 108
#define CHECK(sts,msg) if ((sts) == -1 )  { perror(msg);_exit(-1);}

#define carteEnDebutDePartie 7

//structure Tas de cartes, pour la pioche, le talon ou les mains des joueurs
typedef struct  
{
	char num;
	char coul;
} carte, *cartes;

//structure representant un joueur (avec son pid, pseudo, score, sa main, et son nombre de cartes
typedef struct  
{
	int pid;
	char pseudo[MaxPseudo];
	int score;
	carte jeu [NbCartes];
	int nbCartesEnMain; 
}joueur;

typedef struct partie
{
	//bal
	long type;	// 1)Cli->Serv	2)Serv->Cli
	char msg[256];
	
	joueur joueurs[MaxJoueurs];
	int nbJoueur;
	
	int tour;
	int sens; // 0 = haut en bas, 0 à infini // 1 = bas en haut
	
	carte talon [NbCartes];
	carte pioche [NbCartes];
	
	int indexPioche;
	int indexTalon;
}partie;
 

//definition des fonctions :
void setJoueur(int index, char* pseudo, int pid, int score, int nbCartes /*,cartes jeu*/);
void ajoutJoueurPid (int signal_number, siginfo_t *info);
void * AttenteJoueurs();
void CreerPartie();
void RejoindrePartie (int signal_number, siginfo_t *info);
void DemandeRejoindrePartie(int pidServer, char *tempPseudo);

/************************************************************************************************/
/*Fonction : distribution									*/
/* Description : distribue les cartes aux joueurs en debut de parties en créeant des segments	*/
/*		pour chaque joueurs (portant leurs pid comme nom)				*/
/************************************************************************************************/
void distribution();

/************************************************************************************************/
/*Fonction : clrscr										*/
/* Description : Clear Screen									*/
/************************************************************************************************/
void clrscr();

/************************************************************************************************/
/*Fonction : Enjeu										*/
/* Description : retourne 1 si la partie doit toujours se jouer					*/
/************************************************************************************************/
int Enjeu();

/************************************************************************************************/
/*Fonction : Affichage										*/
/* Description : Affichage du jeu								*/
/************************************************************************************************/
int Affichage();

/************************************************************************************************/
/*Fonction : PartieEnCoursServeur								*/
/* Description : joue une partie avec les joueurs connectés					*/
/************************************************************************************************/
int PartieEnCours( );

/************************************************************************************************/
/*Fonction : PartieEnCoursClient								*/
/* Description : joue une partie avec les joueurs connectés					*/
/************************************************************************************************/
int PartieEnCoursClient( );

/************************************************************************************************/
/*Fonction : carteJouable									*/
/* Description : retourne 0 si la carte n'est pas jouable sinon retourne 1			*/
/************************************************************************************************/
int carteJouable();

/************************************************************************************************/
/*Fonction : whoIam										*/
/* Description : retrouve où on se trouve parmis la liste de joueur				*/
/************************************************************************************************/
int whoIam();


