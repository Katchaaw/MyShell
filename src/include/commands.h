#ifndef COMMANDS_H
#define COMMANDS_H

/**
 * Exécute une commande après avoir remplacé la variable `$F` par un fichier donné.
 *
 * @param cmd La commande à exécuter (peut contenir `$F` à remplacer).
 * @param file Le chemin du fichier à substituer à `$F`.
 * @return Le code de retour de la commande exécutée.
 */
int execute_command(const char *cmd, const char *file, const char *directory,char variable);

int execute_from_if(char **args);

#endif
