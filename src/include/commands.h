#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_LENGTH 256

/**
 * Exécute une commande après avoir remplacé la variable une variable (par exemple `$F`) par une valeur donnée.
 *
 * @param cmd La commande à exécuter (peut contenir une variable à remplacer).
 * @param file Le fichier ou valeur utilisé pour remplacer la variable dans la commande.
 * @param directory Le répertoire associé au fichier (NULL si non nécessaire).
 * @param variable Le caractère représentant la variable à remplacer (ex: 'F' pour `$F`).

 * @return Le code de retour de la commande exécutée.
 */
int execute_command(const char *cmd, const char *file, const char *directory, char variable);

/**
 * Convertit un tableau d'arguments en une chaîne de commande unique.
 *
 * @param args Tableau de chaînes représentant les arguments.

 * @return Une chaîne contenant tous les arguments concaténés, séparés par des espaces.
 */
char *args_to_cmd(char **args);


/**
 * Exécute une commande depuis une structure `if`.
 *
 * @param args Tableau d'arguments représentant la commande à exécuter.
 
 * @return Le code de retour de la commande exécutée.
 */
int execute_from_if(char **args);


/**
 * Remplace toutes les occurrences d'une variable dans une commande par une valeur donnée.
 *
 * @param command La commande contenant la variable à remplacer.
 * @param variable Le caractère représentant la variable (ex: 'F' pour `$F`).
 * @param replacement La valeur qui remplace la variable.
 */
void replaceVariable(char *command, char variable, const char *replacement);

#endif