#ifndef FOR_H
#define FOR_H

/**
 * Parcourt un répertoire et exécute une commande pour chaque fichier non caché.
 * 
 * @param rep Le chemin du répertoire à parcourir.
 * @param cmd La commande à exécuter pour chaque fichier.
 * @param opt_A Active l'inclusion des fichiers cachés, sauf "." et "..".
 * @param opt_r Active la récursivité dans les sous-répertoires.
 * @param opt_ext Filtre les fichiers par extension.
 * @param opt_type Filtre les fichiers par type.
 * @param variable Variable utilisée dans la commande.

 * @return Le code de retour le plus élevé parmi toutes les commandes exécutées, ou 1 en cas d'erreur.
 */
int fsh_for(const char *rep, const char *cmd,int opt_A, int opt_r,const char *opt_ext,char opt_type,char variable);


/**
 * Analyse et exécute une boucle `for` en fonction des arguments donnés.
 * 
 * @param arg Les arguments de la boucle for.
 * @param last_return Pointeur vers la valeur de retour de la dernière commande exécutée.

 * @return 0 ou 1 en cas d'erreur.
 */
int handle_for(char *arg, int *last_return);


#endif
