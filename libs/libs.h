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

#define SIZE 1000
#define MSIZE 64

#define MaxPseudo 20
#define MaxJoueurs 10
#define CHECK(sts,msg) if ((sts) == -1 )  { perror(msg);_exit(-1);}

typedef struct  
{
	int num;
	char coul;
} carte, *cartes;

typedef struct  
{
	int pid;
	char pseudo[MaxPseudo];
	int score;
	cartes jeu;
	int nbCartes;
} joueur;


