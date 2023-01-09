#include "libs.h"


//droit utilisateur UNIX des boites au lettres, exemples : 0760
const mode_t RIGHTS = 0666; //ici droit en lecture/écriture pour tous.

/************************************************************************************************/
/*Fonction : createBox										*/
/* Description : Créer une boites aux lettres avec les droits RIGHTS, retourne son id		*/
/************************************************************************************************/
int createBox( char * name ){
	
	//création d'une clé
	key_t laClef;
	laClef = ftok(name,1);
	
	// Creation de la boite aux lettres
	int boxId = msgget(laClef,IPC_CREAT | IPC_EXCL | RIGHTS );
	//printf("Création de la boite n° %d, erreur : %s\n",boxId,strerror(errno));
	
	return boxId;
}

/************************************************************************************************/
/*Fonction : connecteBox										*/
/* Description : Créer une boites aux lettres avec les droits RIGHTS, retourne son id		*/
/************************************************************************************************/

/************************************************************************************************/
/*Fonction : infoBox										*/
/* Description : Donne des informations sur une boites aux lettres via son id			*/
/************************************************************************************************/
int infoBox(int boxId)
{
	struct msqid_ds buf;

	msgctl(boxId,IPC_STAT,&buf);

	printf("Informations sur la boite aux lettres n° %d  \n",boxId);
	printf("msg_stime %s \n",ctime(&(buf.msg_stime)));
	printf("msg_rstime %s \n",ctime(&(buf.msg_rtime)));
	printf("msg_ctime %s \n",ctime(&(buf.msg_ctime)));
	printf("msg_qnum %ld \n",buf.msg_qnum);
	printf("msg_qbytes %ld \n",buf.msg_qbytes);
	printf("msg_lspid %d \n",buf.msg_lspid);
	printf("msg_lrpid %d \n",buf.msg_lrpid);

	return buf.msg_qnum;
}

/************************************************************************************************/
/*Fonction : writeBox										*/
/* Description : Envoie un message dans une boite aux lettres					*/
/************************************************************************************************/
void writeBox( int boxId, struct partie objet){

	CHECK(msgsnd(boxId,&objet,sizeof(objet),0), "msgsnd");
}

/************************************************************************************************/
/*Fonction : readBox										*/
/* Description : Lis un message dans une boite aux lettres					*/
/************************************************************************************************/
struct partie readBox( int boxId ){
	
	struct partie objet;
	//CHECK(msgrcv(boxId, &objet, sizeof(objet), 1, 0),"msgrcv");
	msgrcv(boxId, &objet, sizeof(struct partie), 1, 0);
        return objet;
}

/*struct t_objet readBox( int boxId ){
	
	struct t_objet objet;
	//CHECK(msgrcv(boxId, &objet, sizeof(objet), 1, 0),"msgrcv");
	msgrcv(boxId, &objet, sizeof(objet), 1, 0);
        return objet;
}*/
/************************************************************************************************/
/*Fonction : deleteBox										*/
/* Description : Détruit une boite aux lettres							*/
/************************************************************************************************/
void deleteBox( int boxId ){
	
	struct msqid_ds buf;
	
	//suppression boite
	if (msgctl(boxId, IPC_RMID, &buf) == -1)
    	{
        	perror("Erreur de suppresion de la boite : %s\n");
        	exit(EXIT_FAILURE);
    	}
    	//printf("Suppression boite aux lettres : %s\n",strerror(errno));
}
