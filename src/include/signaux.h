#ifndef SIGNAUX_H
#define SIGNAUX_H

#include <signal.h>

/**
 * Variable globale indiquant que la commande précédente a été interrompue 
 * par SIGINT ou SIGTERM.
 */
extern volatile sig_atomic_t was_interrupted;

/**
 * Cette fonction est appelée lorsqu'un signal SIGINT ou SIGTERM est reçu. Elle met à jour
 * la variable globale 'was_interrupted' et enregistre le dernier signal reçu.
 *
 * @param signal Le signal reçu.
 */
void handle_signal(int signal);

/**
 * Configure l'installation du gestionnaire pour SIGINT et SIGTERM en utilisant sigaction.
 * La fonction définit l'option SA_RESTART pour relancer les appels système interrompus.
 */
void setup_signals();

/**
 * Remet en place le comportement par défaut pour SIGINT et SIGTERM.
 */
void restore_default_signals();

/**
 * Renvoit une chaîne de caractères correspondant au nom du signal passé en paramètre.
 *
 * @param signal Le signal dont on souhaite obtenir le nom.
 * @return Une chaîne de caractères décrivant le signal ou "UNKNOWN" si le signal n'est pas reconnu.
 */
const char* get_signal_name(int signal);

#endif 
