/** **********************************
 * \file      my_sig.h
 * \author    Loïc PETITPREZ
 * \version   1.0
 * \date      29/09/22
 * \brief     Bibliothèque Personnel de gestion de signal - basé sur <signal.h>
 * **********************************/

//definitions :
void init_sig( struct sigaction* newact, void (*func)() );
