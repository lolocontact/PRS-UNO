//definitions :

int connecteSegment(int size, char *name, int cle /*1,2*/);

int creerSegment(int size, char *name, int cle /*1,2*/);

void supprimerSegment(int shmid);

void detacherSegment(char * segmentADDRESS);

void ecritureSegment(int segmentID, char* message);

char * lireSegment(int shmId);

char * attacheSegment(int segmentID);



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
char * pid2string( int pid );
