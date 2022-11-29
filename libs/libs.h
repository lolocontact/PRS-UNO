#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>  
#include <errno.h>
#include <signal.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <time.h>
//#include <semaphore.h>

#define SIZE 1000
#define MSIZE 64

#define MaxPseudo 20
#define MaxJoueurs 10
#define NbCartes 108
#define CHECK(sts,msg) if ((sts) == -1 )  { perror(msg);_exit(-1);}

//structure Tas de cartes, pour la pioche, le talon ou les mains des joueurs
typedef struct  
{
	int num;
	char coul;
} carte, *cartes;

//structure representant un joueur (avec son pid, pseudo, score, sa main, et son nombre de cartes
typedef struct  
{
	int pid;
	char pseudo[MaxPseudo];
	int score;
	cartes jeu;
	int nbCartesEnMain; 
} joueur;

typedef struct
{
	joueur joueurs[MaxJoueurs];
	int nbJoueur;
	
	int tour;
	int sens;
	
	carte talon [NbCartes];
	carte pioche;
}partie;

//definition des fonctions :
void setJoueur(int index, char* pseudo, int pid, int score, int nbCartes /*,cartes jeu*/);
void ajoutJoueurPid (int signal_number, siginfo_t *info);
void * AttenteJoueurs();
void CreerPartie();
void RejoindrePartie (int signal_number, siginfo_t *info);
void DemandeRejoindrePartie(int pidServer, char *tempPseudo);
