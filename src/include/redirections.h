#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

/**
 * Gère les redirections dans les tokens de commande, en modifiant les descripteurs de fichiers
 * pour rediriger l'entrée et la sortie standard ou l'erreur vers un fichier.
 *
 * @param tokens Tableau de tokens à analyser.
 * @param nb_tokens Pointeur vers le nombre actuel de tokens.
 *
 * @return 0 ou 1 en cas d'erreur.
 */
int handle_redirections(char **tokens, int *nb_tokens);

/**
 * Sauvegarde les descripteurs de fichiers standards pour les redirections.
 */
void save_redirections();


/**
 * Restaure les descripteurs de fichiers standards à leur état d'origine.
 */
void reset_redirections();


/**
 * Restaure les descripteurs de fichiers standards à leur état d'origine.
 */
void close_saved_redirections();

#endif
