/** **********************************
 * \file      my_tube.h
 * \author    Loïc PETITPREZ
 * \version   1.0
 * \date      29/09/22
 * \brief     Bibliothèque Personnel de gestion de tube
 * **********************************/

//definitions :
void ajouterUtilisateur(int newPid);
void * connexions ( void* DonneeServeur );
void connexionsHandler (int signal_number, siginfo_t *info);

void createTube( char * name );
int connectTubeRead( char * name );
void readTube( int valread, char * buffer, int size /*MSIZE*/);
int connectTubeWrite( char * name );
void writeTube( int valwrite, char * buffer, int size /*MSIZE*/);
char * nameTube( int pid );


