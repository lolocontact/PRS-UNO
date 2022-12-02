#include "libs.h"

/**************************************************************************************************
\Author : Loïc Petitprez
\brief remplie une structure pioche à partir d'un fichier
**************************************************************************************************/
void chargement(char *fileName, carte pioche[NbCartes]){
	//le type FILE
	FILE *fic=NULL; 
	//compteurs de charactère
	int i=0;
	int j=0;
	//variables de récuperation
	char coulTemp;
	int nbrTemp;
	char numTemp;
	
	// r = le mode read, fopen renvoie NULL si probleme (disque plein, disque non accessible ...
	fic=fopen(fileName,"r"); 
	
	if (fic==NULL){
		printf("ERROR FATAL : file not found (-1) \n");
		exit(-1);
	}

	while(!feof(fic)){
		
		//type de saisie dans le document : <nbr> <espace> <num> <espace> <coul> <retour chariot>
		
		//on récupère le nombre d'une carte en un char (coulTemp sert de variable de transition)
		fread(&coulTemp, sizeof(char), 1, fic);
		//et on le convertit en int
		nbrTemp = coulTemp - '0';
		
		//on avance de 1 le pointeur stream (pour retirer un espace)
		fgetc(fic);
		
		//on récupère le numéro d'une carte en un char (coulTemp sert de variable de transition)
		fread(&numTemp, sizeof(char), 1, fic);
		//et on le convertit en int
		//numTemp = coulTemp - '0';
		
		//on avance de 1 le pointeur stream (pour retirer un espace)
		fgetc(fic);
		
		//on récupère la couleur d'une carte en un char
		fread(&coulTemp, sizeof(char), 1, fic);
		
		//on passe le retour chariot
		fgetc(fic);
		
		for(j=0; j<nbrTemp; j++){
			pioche[i+j].num = numTemp;
			pioche[i+j].coul = coulTemp;
		}
		
		i=i+j;
	}
	
	//fermeture fichier
	fclose(fic);
	
	/*printf("Affichage pioche\n");
	
	for(j=0; j<NbCartes; j++){
		printf("num: %c | coul: %c\n",pioche[j].num,pioche[j].coul);
	}
	printf("%d",j);*/
}

/**************************************************************************************************
\Author : Loïc Petitprez
\brief mélande un tas de carte
**************************************************************************************************/
void shuffle(carte tas[NbCartes])
{
    if (NbCartes > 1) {
        int i;
        for (i = NbCartes - 1; i > 0; i--) {
            int j = (unsigned int) (drand48()*(i+1));
            carte t = tas[j];
            tas[j] = tas[i];
            tas[i] = t;
        }
    }
}
