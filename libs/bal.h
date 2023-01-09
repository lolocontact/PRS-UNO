struct t_objet {
        long type;
        pid_t pid;
        char msg[256];
};

#define MAXMSG 100

/************************************************************************************************/
/*Fonction : createBox										*/
/* Description : Créer une boites aux lettres avec les droits RIGHTS, retourne son id		*/
/************************************************************************************************/
int createBox( char * name );

/************************************************************************************************/
/*Fonction : infoBox										*/
/* Description : Donne des informations sur une boites aux lettres via son id			*/
/************************************************************************************************/
int infoBox(int boxId);

/************************************************************************************************/
/*Fonction : writeBox										*/
/* Description : Envoie un message dans une boite aux lettres					*/
/************************************************************************************************/
void writeBox( int boxId, struct partie objet);

/************************************************************************************************/
/*Fonction : readBox										*/
/* Description : Lis un message dans une boite aux lettres					*/
/************************************************************************************************/
struct partie readBox( int boxId );

/************************************************************************************************/
/*Fonction : deleteBox										*/
/* Description : Détruit une boite aux lettres							*/
/************************************************************************************************/
void deleteBox( int boxId );
