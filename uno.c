#include "libs/libs.h"
#include "libs/segment.h"
#include "libs/my_sig.h"
#include "libs/my_tube.h"

//=================================================Variables Globales==================================================
joueur joueurs[MaxJoueurs];
int nbJoueur = 0;

int PidServeur = 0;

pthread_t thread;
void * ret;
static pthread_mutex_t  mutexAttenteServ = PTHREAD_MUTEX_INITIALIZER;

char tempPseudo[MaxPseudo];
int tempReponse;

/************************************************************************************************/
/*Fonction : setJoueur										*/
/* Description : ajoute un joueur dans le tableau joueurs					*/
/************************************************************************************************/
void setJoueur(int index, char* pseudo, int pid, int score, int nbCartes /*,cartes jeu*/){
	joueurs[index].pid = pid;
	strcpy(joueurs[index].pseudo, pseudo);
	joueurs[index].score = score;
	joueurs[index].nbCartes = nbCartes;
}

//=======================================================================================================
//---------------------------------------------Coté Hébergeur--------------------------------------------
//=======================================================================================================

/************************************************************************************************/
/*Fonction : ajoutJoueurPid									*/
/* Description : ajoute le pid du nouveau joueur						*/
/************************************************************************************************/
void ajoutJoueurPid (int signal_number, siginfo_t *info)
{
	switch (signal_number) {
		case SIGUSR1 : 
			//printf("Signal SIGUSR1 reçu : %d.\n",info->si_pid);
			if(nbJoueur<MaxJoueurs){
				nbJoueur ++;
				setJoueur(nbJoueur, "none", info->si_pid, 0, 0);
				//printf("pid : %d\n", info->si_pid);
				kill(joueurs[nbJoueur].pid, SIGUSR1);
				
				//lecture segment (pseudo)
				sleep(1);
				int segmId = connecteSegment( MaxPseudo * sizeof(char), pid2string( info->si_pid ) , 2 );
				setJoueur(nbJoueur, lireSegment(segmId), info->si_pid, 0, 0);
				printf("- %s\n",joueurs[nbJoueur].pseudo);
				
				//printf("pid : %d",joueurs[nbJoueur].pid);
			}
			else{
				printf("Tentative de connexion mais nombre de joueurs limite atteint \n");
			}
			
			break;
		default:
			break;

	}
}

/************************************************************************************************/
/*Fonction : AttenteJoueurs									*/
/* Description : ajoute et communique avec un joueur qui vient de se connecter			*/
/************************************************************************************************/
void * AttenteJoueurs() {
	// Attente signal (SIGUSR1)
	struct sigaction newact;
	init_sig(&newact, ajoutJoueurPid);
	
	CHECK(sigaction(SIGUSR1, &newact, NULL),"problème sigaction sur SIGUSR1");
	
	
	int attente = 1;
	scanf("%d", &attente);
	
	
	pthread_exit("msg"); 
}

/************************************************************************************************/
/*Fonction : CreerPartie									*/
/* Description : Initialise une partie et attends les autres joueurs				*/
/************************************************************************************************/
void CreerPartie(){
	printf("-------------- code à partager : %d --------------\n", joueurs[0].pid);
	printf("-------------- LISTE DES JOUEURS (entrez n'importe quel charactere pour débuter) --------------\n");
	printf("- %s\n", joueurs[0].pseudo);
	
	pthread_create(&thread, NULL, AttenteJoueurs, NULL);
	pthread_join(thread, &ret);
}


//=======================================================================================================
//---------------------------------------------Coté Joueur-----------------------------------------------
//=======================================================================================================

/************************************************************************************************/
/*Fonction : RejoindrePartie									*/
/* Description : Rejoins une partie en créeant un tube entre les 2 pour les pseudos		*/
/************************************************************************************************/
void RejoindrePartie (int signal_number, siginfo_t *info){
	switch (signal_number) {
		case SIGUSR1 : 
			PidServeur = info->si_pid;
			pthread_mutex_unlock(& mutexAttenteServ);
			break;
		default:
			printf("Connexion refusé\n");
			exit(0);
			break;

	}

}

/************************************************************************************************/
/*Fonction : DemandeRejoindrePartie								*/
/* Description : Demande de Rejoindre une partie en envoyant un signal à l'hébergeur		*/
/************************************************************************************************/
void DemandeRejoindrePartie(int pidServer, char *tempPseudo){
	
	//envoie signal
	kill(pidServer, SIGUSR1);
	// Attente signal (SIGUSR1)
	struct sigaction newact;
	init_sig(&newact, RejoindrePartie);
	
	CHECK(sigaction(SIGUSR1, &newact, NULL),"problème sigaction sur SIGUSR1");
	
	//attente valisation de l'hébergeur
	pthread_mutex_lock(& mutexAttenteServ);
	
	createTube(nameTube(getpid()));
	//TODO
	int segmId = creerSegment( MaxPseudo * sizeof(char), pid2string( getpid() ), 1 );
	ecritureSegment( segmId, tempPseudo);
	
	for( ; ; ){ }
	
}


//=======================================================================================================
//-------------------------------------------------Main--------------------------------------------------
//=======================================================================================================
int main()
{
	printf("Veuillez saisir votre pseudo :\n");
	scanf("%s",tempPseudo);
	
	printf("Que voulez-vous faire ?\n");
	printf("1) Créer/héberger une partie\n");
	printf("2) Rejoindre une partie\n");
	scanf("%d",&tempReponse);
	
	if(tempReponse == 1){
		nbJoueur ++;
		PidServeur = getpid();
		setJoueur(0, tempPseudo, getpid(), 0, 0);
		CreerPartie();
	}
	if(tempReponse == 2){
		printf("Veuillez saisir le Code Hébergeur :\n");
		scanf("%d",&tempReponse);
		DemandeRejoindrePartie(tempReponse, tempPseudo);
		
	}
	
	return 0;
}

/*
CreerPartie(){
    Initialise attente = 1;
    Afficher PID, pseudo de joueurs[0];
    Tant que Attente == 1
        créer thread AttenteJoueurs
        Saisir Attente
    Fin thread AttenteJoueurs
    Créer Segment( joueurs.fifo )
    Ecrire sur joueurs.fifo
    Envoyer signal USR2 aux joueurs
    //debut jeu 
}

thread AttenteJoueurs{
    Attente signal (SIGUSR1)
    Renvoie signal (SIGUSR1)
    lis tube joueur n
    nbJoueurs ++
    pidJoueur,pseudoJoueur,0,0 -> joueurs[nbJoueurs];
}

RejoindrePartie(){
    Saisir code partagé (PID hebergeur)
    envoie signal USR1 au PID
    Attente signal USR1
    enregistrer le pid du serveur
    créer tube joueur n
    envoyer sur joueurs n (pid, pseudo)
    Attente signal USR2
    //debut partie
}


*/




