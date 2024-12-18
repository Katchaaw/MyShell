#ifndef FOR_H
#define FOR_H

/**
 * Parcourt un répertoire et exécute une commande pour chaque fichier non caché.
 * 
 * @param rep Le chemin du répertoire à parcourir.
 * @param cmd La commande à exécuter pour chaque fichier.
 * @return 0 si la commande a été exécutée pour tous les fichiers, 1 en cas d'erreur.
 */
int fsh_for(const char *rep, const char *cmd,int opt_A, int opt_r,const char *opt_ext);


int handle_for(char *arg, int *last_return);


#endif
