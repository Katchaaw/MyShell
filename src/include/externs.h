#ifndef EXTERNS_H
#define EXTERNS_H

/**
 * Indique si la dernière commande a été terminée par un signal.
 *
 * - `0` : Commande terminée normalement.
 * - `1` : Commande interrompue par un signal.
 * - `2` : Commande interrompue par `SIGINT` (Ctrl+C).
 *
 * Cette variable est mise à jour après chaque exécution de commande externe.
 */
extern int last_was_signal;

/**
 * Exécute une commande externe en utilisant `execvp`.
 *
 * @param cmd Nom de la commande à exécuter.
 * @param args Tableau d'arguments pour la commande (le premier élément est `cmd`).
 
 * @return Le code de retour de la commande exécutée ou 1 en cas d'erreur.
 */
int execute_external_command(char *cmd, char **args);

#endif