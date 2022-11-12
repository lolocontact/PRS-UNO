#include "libs.h"
/** **********************************
 * \file      my_sig.h
 * \author    Loïc PETITPREZ
 * \version   1.0
 * \date      29/09/22
 * \brief     Bibliothèque Personnel de gestion de signal - basé sur <signal.h>
 * **********************************/

/**************************************************************************************************/
/*Fonction : init_sig                                                                             */
/* Description : parametre un sigaction (fonction, mask, check)                                   */
/* Entrees : pointeur vers une fonction                                                           */
/* Sorties : retourne le sigaction parametré							  */
/**************************************************************************************************/
void init_sig( struct sigaction* newact, void (*func)() ){
	
	
	//on initialise la fonction
	newact->sa_handler = (*func);
	//(*func)(param...)
	
	// Re-initialisation du masque  de traitement des signaux
	CHECK(sigemptyset(&(newact->sa_mask)),"problème sigemptyset"); 

	//on accède aux infos disponible sur l'envoyeur
	newact->sa_flags = SA_SIGINFO;
	//ici mask par default
	sigprocmask(SIG_SETMASK,&(newact->sa_mask),NULL);
}



//MASK CF 223.c
/*//mask :
	sigset_t new_mask;
	sigset_t old_mask;

	// initialise un nouveau mask
	sigfillset(&new_mask);

	//bloque les signaux
	sigprocmask(SIG_SETMASK, &new_mask, &old_mask);
	
	//restore signal mask
	//sigprocmask(SIG_SETMASK, &old_mask, NULL);
*/
