#include "libs/libs.h"
#include "libs/segment.h"
#include "libs/my_sig.h"
#include "libs/my_tube.h"
#include "libs/my_file.h"
#include "libs/bal.h"

//=================================================Variables Globales==================================================
//liste des joueurs
struct partie p;
struct partie* pp = &p;
volatile int shmtP;
int indexMe;

//pid du serveur (dans le cas où l'on rejoins une partie)
volatile int PidServeur = 0;
int boxId;

//threads
pthread_t thread;
void * ret;
//sem_t  mutexAttenteServ ;//= PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexAttenteServ = PTHREAD_MUTEX_INITIALIZER; //mutex attente serveur
pthread_cond_t servIsOk	= PTHREAD_COND_INITIALIZER; 

static pthread_mutex_t  mutexAttenteJoueurs = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t startGame = PTHREAD_COND_INITIALIZER;

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
int initPartie(){
	p.tour = 0;
	p.sens = 0; //haut en bas, 0 à infini
	
	//init index
	p.indexTalon = 0;
	p.indexPioche = 0;
	
	//on charge les cartes à partir du fichier uno.txt
	chargement("uno.txt", p.pioche);
	//on mélange le jeu de cartes
	shuffle(p.pioche);
	
	//création boite aux lettres
	return createBox(CLE);
}

/************************************************************************************************/
/*Fonction : Enjeu										*/
/* Description : retourne 1 si la partie doit toujours se jouer	sinon retourne le gagnant	*/
/************************************************************************************************/
int Enjeu(){
	for(int i=0; i<=p.nbJoueur; i++){
		if(p.joueurs[i].nbCartesEnMain == 0){
			return i;
		}
	}
	return 1;
}

/************************************************************************************************/
/*Fonction : whoIam										*/
/* Description : retourne le numéros du joueur							*/
/************************************************************************************************/
int whoIam(){
	for(int i=0; i<=p.nbJoueur; i++){
	//printf("whoIam getpid : %d",getpid());
		if(p.joueurs[i].pid == getpid()){
			indexMe = i;
			return i;
		}
	}
	return 0;
}

/************************************************************************************************/
/*Fonction : clrscr										*/
/* Description : Clear Screen									*/
/************************************************************************************************/
void clrscr()
{
    system("@cls||clear");
}

/************************************************************************************************/
/*Fonction : Affichage										*/
/* Description : Affichage du jeu								*/
/************************************************************************************************/
int Affichage(){
	
	clrscr();
	
	int i; int index = 0;
	//printf("Il y a %d joueurs\n",p.nbJoueur+1);
	for(i=0; i<=p.nbJoueur; i++){
		printf("-%s : score %d pts : %d cartes  -PID: %d\n", p.joueurs[i].pseudo, p.joueurs[i].score, p.joueurs[i].nbCartesEnMain, p.joueurs[i].pid);
		if(strcmp(p.joueurs[i].pseudo, tempPseudo) == 0 ){
			index = i;
		}
	}
	printf("c'est au tour de : %s\n", p.joueurs[ p.tour ].pseudo);
	if(p.sens == 1){
		printf("- Sens de bas en haut\n\n\n");
	}else{
		printf("- Sens de haut en bas\n\n\n");
	}
	
	printf("- Talon : %c %c \n",p.talon[p.indexTalon].num, p.talon[p.indexTalon].coul);
	printf("- Main : ");
	for(i=0; i<p.joueurs[index].nbCartesEnMain; i++){
		printf("%d) %c%c\t", i, p.joueurs[index].jeu[i].num, p.joueurs[index].jeu[i].coul);
	}
	printf("%d) pioche\t", p.joueurs[index].nbCartesEnMain);
	
	/*if(p.tour == index ){
		scanf("%d", &i);
		return i;
	}
	for(i=p.indexPioche; i<108; i++){
		printf("%d) %c%c\t", i, p.pioche[i].num, p.pioche[i].coul);
	}*/
	
	return 0;
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
void ajoutJoueurPid (int signal_number, siginfo_t *info){
	//partie pTemp = lecturePartie(shmtP);
	switch (signal_number){
		case SIGUSR1 : 
		
			//si on peut acceuillir le joueur au sein de la partie
			//if(pTemp.nbJoueur<MaxJoueurs){
			if(p.nbJoueur<MaxJoueurs){
				//on ajoute le joueur
				//pTemp.nbJoueur ++;
				//pp->nbJoueur ++;
				p.nbJoueur ++;
				//setJoueur(p.nbJoueur, "none", info->si_pid, 0, 0);
				//(&(pp->joueurs[pp->nbJoueur]))->pid = info->si_pid;			
				
				//on le préviens qu'il est prit (signal SIGUSR1)
				kill( info->si_pid, SIGUSR1);
				
				sleep(1);
				//on récupère le pseudo du joueur (via un segment partagée)
				//int segmId = connecteSegment( MaxPseudo * sizeof(char), pid2string( info->si_pid ) , 2 );
				//setJoueur(nbJoueur, lireSegment(segmId), info->si_pid, 0, 0);
				
				//on récupère le pseudo du joueur (via un tube)
				char tempString [MaxPseudo];
				int valread = readTube( nameTube(info->si_pid), tempString, MaxPseudo);
				close(valread);
				
				//setJoueur(pp->nbJoueur, tempString, info->si_pid, 0, 0);
				//setJoueur(p.nbJoueur, tempString, info->si_pid, 0, 0);
				p.joueurs[p.nbJoueur].pid = info->si_pid;
				strcpy(p.joueurs[p.nbJoueur].pseudo, tempString);
				p.joueurs[p.nbJoueur].score = 0;
				p.joueurs[p.nbJoueur].nbCartesEnMain = 0;
				
				printf("- %s\n",p.joueurs[p.nbJoueur].pseudo);
				
				//set p
				//ecriturePartie(shmtP, pTemp);
				
				//printf("- %s\n",(&(pp->joueurs[pp->nbJoueur]))->pseudo);
	
				//TODO pthread_cond_signal(&servIsOk);
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
	pthread_cond_signal(&startGame);
	
	pthread_exit("msg"); 
}

/************************************************************************************************/
/*Fonction : CreerPartie									*/
/* Description : Initialise une partie et attends les autres joueurs				*/
/************************************************************************************************/
void CreerPartie(){
	
	//on charge les cartes à partir du fichier uno.txt
	chargement("uno.txt", p.pioche);
	//on mélange le jeu de cartes
	shuffle(p.pioche);
	
	//création boite aux lettres
	boxId = createBox(CLE);	
	
	printf("%d",boxId);
	
	//creation segment pour partager le numéro de boite aux lettres
	int segmId = creerSegment( sizeof(int), pid2string( getpid() ) , 2 );
	printf("segmId %d\n",segmId);
	ecritureSegment(segmId, boxId);
	
	//création segment pour partager p
	//shmtP = creerSegment( sizeof(int), "/tmp/p" , 2 );
	//ecriturePartie(shmtP, p);
	
	printf("-------------- code à partager : %d (boxId : %d)--------------\n", p.joueurs[0].pid, boxId);
	printf("-------------- LISTE DES JOUEURS (entrez n'importe quel charactere pour débuter) --------------\n");
	printf("- %s\n", p.joueurs[0].pseudo);
	
	//thread attendant les joueurs et l'approbation de l'hébergeur pour démarrer.
	pthread_create(&thread, NULL, AttenteJoueurs, NULL);
	//p = readBox(boxId);
	
	pthread_cond_wait(&  startGame, &  mutexAttenteJoueurs);
	//TODO pthread_cond_wait(&servIsOk, &mutexAttenteServ);	
	pthread_join(thread, &ret);
	
	//init partie	
	p.tour = 0;
	p.sens = 0; //haut en bas, 0 à infini
	p.joueurs[0].nbCartesEnMain = 0; //init joueur hébergeur
	
	//init index
	p.indexPioche = 0;
	p.indexTalon = 0;
	
	//--distribution des cartes
	//pour chaque joueur :
	for(int j=0; j<=p.nbJoueur; j++){
		//on donne 7 cartes
		for(int i=0; i < carteEnDebutDePartie; i++){
			p.joueurs[j].jeu[i] = p.pioche[p.indexPioche];
			//printf("\t%c%c", p.joueurs[j].jeu[i].num, p.joueurs[j].jeu[i].coul);
			p.joueurs[j].nbCartesEnMain ++;
			p.indexPioche ++;
		}
	}
	
	//talon :
	p.talon[0]= p.pioche[p.indexPioche];
	p.indexPioche ++;
	
	//init type
	p.type = 1;
	
	//msgsnd(boxId,&p,sizeof(partie),0);
	writeBox( boxId,p);
	Affichage();
	
	PartieEnCours();
	
	deleteBox(boxId);	
	
	//writeBox(boxId, p);
	//PartieEnCoursServeur();	
	
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
			printf("Avant mutex débloque\n");
			pthread_cond_signal(&servIsOk);
			//sem_post(&mutexAttenteServ);
			printf("Après mutex débloque\n");
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
	//sem_wait(&mutexAttenteServ);
	pthread_cond_wait(&servIsOk, &mutexAttenteServ);
	
	//on récupère l'id de la boite aux lettres
	int segmId = connecteSegment( sizeof(int), pid2string( PidServeur ), 2 );
	boxId = lireSegment(segmId);
	//printf("----boxId %d ----\n",boxId);
	
	//scanf("%d",&tempReponse);
	
	//msgrcv(boxId, &p, sizeof(struct partie), 1, 0);
	p = readBox(boxId);
	
	Affichage();
	
	//TODO démarrer la partie
	PartieEnCours();
	
	
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
		//p.nbJoueur ++;
		PidServeur = getpid();
		setJoueur(0, tempPseudo, getpid(), 0, 0);
		//on créer la partie
		CreerPartie();
	}
	//si rejoins une partie
	if(tempReponse == 2){
		
		printf("Veuillez saisir le Code Hébergeur :\n");
		scanf("%d",&tempReponse);
		
		//variable temporaire
		//char temp2[MaxPseudo]; char temp3[MaxPseudo];
		//split pid-n°boite
		//printf("%s : %s\n",premier(tempPseudo, temp2), saufpremier(tempPseudo, temp3));
		DemandeRejoindrePartie(tempReponse, tempPseudo);
	}
	
	return 0;
}


/************************************************************************************************/
/*Fonction : Jeu										*/
/* Description : Tour et jeu en cours								*/
/************************************************************************************************/
int PartieEnCours(){
	
	//init perso
	whoIam();
	while(Enjeu()){
		
		
		Affichage();
		printf("Je suis le joueur n°%d / %d \n",indexMe, p.nbJoueur);
		
		//si c'est mon tour
		if( p.joueurs[ p.tour ].pid == getpid() || indexMe == p.tour ){
			printf("C'est à vous de jouer, Veuillez saisir votre carte : \n");
			//saisir carte, tant que coup joué est jouable TODO
			//do{
				scanf("%d",&tempReponse);
			//}while( carteJouable() );
			
			//si on pioche
			if(tempReponse == p.joueurs[indexMe].nbCartesEnMain){
				//pioche
				p.indexPioche ++;
				p.joueurs[indexMe].jeu[p.joueurs[indexMe].nbCartesEnMain] = p.pioche[p.indexPioche];
				p.joueurs[indexMe].nbCartesEnMain ++;
				
			}
			else{
				//maj Talon
				p.indexTalon ++;
				p.talon[p.indexTalon]=p.joueurs[indexMe].jeu[tempReponse];
				
				//on retire la carte de la main
				for(int i=tempReponse; i<p.joueurs[indexMe].nbCartesEnMain; i++)
					p.joueurs[indexMe].jeu[i] = p.joueurs[indexMe].jeu[i+1];
					
				//maj de l'index
				p.joueurs[indexMe].nbCartesEnMain --;
			}
			
			//on change de tour selon le sens // 0 = haut en bas, 0 à infini // 1 = bas en haut
			if(p.sens==0){
				if(p.tour == p.nbJoueur){
					p.tour = 0;
				}else{
					p.tour = p.tour+1;
				}
			}else{
				if(p.tour == 0){
					p.tour = p.nbJoueur;
				}else{
					p.tour = p.tour-1;
				}
			}
			
			//publie p
			writeBox(boxId, p);
		}
		//si ce n'est pas mon tour
		else{
			//Attente publication p (changement dans la partie)
			p=readBox(boxId);
		}
	}
	
	//clrscr();
	
	printf("LE GAGNANT EST : %s \n",p.joueurs[Enjeu()].pseudo);
	
	return 0;
}



//TODO :
/************************************************************************************************/
/*Fonction : carteJouable									*/
/* Description : retourne 0 si la carte n'est pas jouable sinon retourne 1			*/
/************************************************************************************************/
int carteJouable(){
	if(tempReponse < p.joueurs[indexMe].nbCartesEnMain && tempReponse >= 0){
		//si on pioche : le coup est jouable
		if(tempReponse == p.joueurs[indexMe].nbCartesEnMain)
			return 1;
		//si le coup est jouable
		if(p.joueurs[indexMe].jeu[tempReponse].num == p.talon[p.indexTalon].num || p.joueurs[indexMe].jeu[tempReponse].coul == p.talon[p.indexTalon].coul )
			return 1;
	}
	//le coup n'est pas jouable
	return 0;

}

