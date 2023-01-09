/**************************************************************************************************
\Author : Loïc Petitprez
\brief remplie une structure pioche à partir d'un fichier
**************************************************************************************************/
void chargement(char *fileName, carte pioche[NbCartes]);

/**************************************************************************************************
\Author : Loïc Petitprez
\brief mélande un tas de carte
**************************************************************************************************/
void shuffle(carte tas[NbCartes]);

/**************************************************************************************************/
/*Fonction : saufpremier                                                                          */
/* Description :        envoie la chaine sans le premier de cette dernière                        */
/* Entrees :            ch la chaine de caractere                                                 */
/* Sorties :            ch sans son premier mot                                                   */
/* Retourne :           ch sans son premier mot                                                   */
/**************************************************************************************************/		
char * saufpremier ( char *ch, char *restch);

/**************************************************************************************************/
/*Fonction : premier                                                                              */
/* Description :        envoie le premier mot d'une chaine                                        */
/* Entrees :            ch la chaine de caractere                                                 */
/* Sorties :            le premier mot de la chaine ch                                            */
/* Retourne :           le premier mot de la chaine ch                                            */
/**************************************************************************************************/
char *premier(char *ch, char *mot);
