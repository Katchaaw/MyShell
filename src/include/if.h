#ifndef IF_H
#define IF_H

#define MAX_CMD_LENGTH 1024 // Taille maximale d'une commande

/**
 * Gère la structure conditionnelle "if ... else" dans le shell.
 *
 * @param tokens Un tableau de chaînes représentant les tokens de la commande.
 * @param nb_tokens Un pointeur vers le nombre de tokens dans la commande.
 * @param last_return Un pointeur vers le code de retour de la dernière commande exécutée.
 *
 * @return 0 ou 1 en cas d'erreur.
 *
 */
int handle_if_else(char **tokens, int *nb_tokens, int *last_return);

#endif
