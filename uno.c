#include "libs/libs.h"
#include "libs/segment.h"
#include "libs/my_sig.h"
#include "libs/my_tube.h"
#include "libs/my_file.h"

//=================================================Variables Globales==================================================
//liste des joueurs
partie p;

//pid du serveur (dans le cas où l'on rejoins une partie)
int PidServeur = 0;

//threads
pthread_t thread;
void * ret;
static pthread_mutex_t  mutexAttenteServ = PTHREAD_MUTEX_INITIALIZER;

//variables temporaire
char tempPseudo[MaxPseudo];
int tempReponse;

/************************************************************************************************/
/*Fonction : setJoueur										*/
/* Description : ajoute un joueur dans le tableau joueurs					*/
/************************************************************************************************/
void setJoueur(int index, char* pseudo, int pid, int score, int nbCartesEnMain /*,cartes jeu*/){
	p.joueurs[index].pid = pid;
	strcpy(p.joueurs[index].pseudo, pseudo);
	p.joueurs[index].score = score;
	p.joueurs[index].nbCartesEnMain = nbCartesEnMain;
}

/************************************************************************************************/
/*Fonction : initPartie										*/
/* Description : initialise une partie de uno de zéro (tour, sens etc...)			*/
/************************************************************************************************/
void initPartie(){
	p.tour = 0;
	p.sens = 0; //haut en bas, 0 à infini
	
	//TODO mélange cartes -> Pioche et Talon
	
}

//=======================================================================================================
//=======================================================================================================
//---------------------------------------------Coté Hébergeur--------------------------------------------
//=======================================================================================================
//=======================================================================================================

/************************************************************************************************/
/*Fonction : ajoutJoueurPid									*/
/* Description : ajoute le pid du nouveau joueur						*/
/************************************************************************************************/
void ajoutJoueurPid (int signal_number, siginfo_t *info)
{
	switch (signal_number) {
		case SIGUSR1 : 
			//si on peut acceuillir le joueur au sein de la partie
			if(p.nbJoueur<MaxJoueurs){
				//on ajoute le joueur
				p.nbJoueur ++;
				setJoueur(p.nbJoueur, "none", info->si_pid, 0, 0);
				
				//on le préviens qu'il est prit (signal SIGUSR1)
				kill(p.joueurs[p.nbJoueur].pid, SIGUSR1);
				
				sleep(1);
				//on récupère le pseudo du joueur (via un segment partagée)
				//int segmId = connecteSegment( MaxPseudo * sizeof(char), pid2string( info->si_pid ) , 2 );
				//setJoueur(nbJoueur, lireSegment(segmId), info->si_pid, 0, 0);
				
				//on récupère le pseudo du joueur (via un tube)
				int valread = readTube( nameTube(info->si_pid), p.joueurs[p.nbJoueur].pseudo, MaxPseudo);
				close(valread);
				
				printf("- %s\n",p.joueurs[p.nbJoueur].pseudo);
				
				//TODO : supprimer le segment partagée, en recréer un pour les échanges pour la partie
			}
			else{
				//on le préviens qu'il n'est pas prit (signal SIGUSR2)
				kill(info->si_pid, SIGUSR2);
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
	
	// Attente signal (SIGUSR1) des joueurs
	struct sigaction newact;
	init_sig(&newact, ajoutJoueurPid);
	
	CHECK(sigaction(SIGUSR1, &newact, NULL),"problème sigaction sur SIGUSR1");
	
	//attente de l'approbation de l'hébergeur afin de démarrer la partie
	int attente = 1;
	scanf("%d", &attente);
	
	pthread_exit("msg"); 
}

/************************************************************************************************/
/*Fonction : CreerPartie									*/
/* Description : Initialise une partie et attends les autres joueurs				*/
/************************************************************************************************/
void CreerPartie(){
	initPartie();
	printf("-------------- code à partager : %d --------------\n", p.joueurs[0].pid);
	printf("-------------- LISTE DES JOUEURS (entrez n'importe quel charactere pour débuter) --------------\n");
	printf("- %s\n", p.joueurs[0].pseudo);
	
	//thread attendant les joueurs et l'approbation de l'hébergeur pour démarrer.
	pthread_create(&thread, NULL, AttenteJoueurs, NULL);
	pthread_join(thread, &ret);
	
	//on charge les cartes à partir du fichier uno.txt
	chargement("uno.txt", p.pioche);
	//on mélange le jeu de cartes
	shuffle(p.pioche);
	shuffle(p.pioche);
	
	for(int j=0; j<NbCartes; j++){
		printf("num: %c | coul: %c\n",p.pioche[j].num,p.pioche[j].coul);
	}
	
	
	
	//TODO démarrer la partie
	
}

//=======================================================================================================
//=======================================================================================================
//---------------------------------------------Coté Joueur-----------------------------------------------
//=======================================================================================================
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
	
	//création de tube afin d'envoyer le pseudo
	createTube(nameTube(getpid()));
	//on écrit le pseudo dans le tube
	int valwrite = writeTube( nameTube( getpid()), tempPseudo, MaxPseudo);
	
	//envoie signal à l'hergeur pour demander l'accès à la partie
	kill(pidServer, SIGUSR1);
	// Attente signal (SIGUSR1) de la part de l'hebergeur (accès accepté)
	struct sigaction newact;
	init_sig(&newact, RejoindrePartie);
	
	CHECK(sigaction(SIGUSR1, &newact, NULL),"problème sigaction sur SIGUSR1");
	
	//attente validation de l'hébergeur
	pthread_mutex_lock(& mutexAttenteServ);
	

	
	
	//création mémoire partagée afin d'envoyer le pseudo
	//int segmId = creerSegment( MaxPseudo * sizeof(char), pid2string( getpid() ), 1 );
	//ecritureSegment( segmId, tempPseudo);
	
	
	//TODO démarrer la partie
	for( ; ; ){ }
	
	
	//on ferme la sortie écriture du tube
	close(valwrite);
}

//=======================================================================================================
//=======================================================================================================
//-------------------------------------------------Main--------------------------------------------------
//=======================================================================================================
//=======================================================================================================
int main()
{
	printf("Veuillez saisir votre pseudo :\n");
	scanf("%s",tempPseudo);
	
	printf("Que voulez-vous faire ?\n");
	printf("1) Créer/héberger une partie\n");
	printf("2) Rejoindre une partie\n");
	scanf("%d",&tempReponse);
	
	//si héberge la partie
	if(tempReponse == 1){
		//init
		p.nbJoueur = 0;
		//on ajoute l'hébergeur en tant que joueur
		p.nbJoueur ++;
		PidServeur = getpid();
		setJoueur(0, tempPseudo, getpid(), 0, 0);
		//on créer la partie
		CreerPartie();
	}
	//si rejoins une partie
	if(tempReponse == 2){
		printf("Veuillez saisir le Code Hébergeur :\n");
		scanf("%d",&tempReponse);
		DemandeRejoindrePartie(tempReponse, tempPseudo);
		
	}
	
	return 0;
}




