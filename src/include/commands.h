#ifndef COMMANDS_H
#define COMMANDS_H

/**
 * Exécute une commande après avoir remplacé la variable `$F` par un fichier donné.
 *
 * @param cmd La commande à exécuter (peut contenir `$F` à remplacer).
 * @param file Le chemin du fichier à substituer à `$F`.
 * @return Le code de retour de la commande exécutée.
 */
int execute_command(const char *cmd, const char *file);


/**
 * Parcourt un répertoire et exécute une commande pour chaque fichier non caché.
 * 
 * @param rep Le chemin du répertoire à parcourir.
 * @param cmd La commande à exécuter pour chaque fichier.
 * @return 0 si la commande a été exécutée pour tous les fichiers, 1 en cas d'erreur.
 */
int fsh_for(const char *rep, const char *cmd);

#endif