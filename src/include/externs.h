#ifndef EXTERNS_H
#define EXTERNS_H

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Exécute une commande externe en utilisant `execvp`.
 *
 * @param cmd Nom de la commande à exécuter.
 * @param args Tableau d'arguments pour la commande (le premier élément est `cmd`).
 * @return Le code de retour de la commande exécutée ou 1 en cas d'erreur.
 */
int execute_external_command(char *cmd, char **args);

#endif