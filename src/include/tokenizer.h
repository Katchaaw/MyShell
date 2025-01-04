#ifndef TOKENIZER_H
#define TOKENIZER_H

// Nombre maximal de tokens.
#define MAX_TOKENS 100

/**
 * Divise une chaîne de caractères en tokens selon un délimiteur donné.
 *
 * @param line La ligne à tokeniser.
 * @param tokens Tableau pour stocker les pointeurs vers les tokens extraits.
 * @param nb_tokens Pointeur vers le compteur de tokens.
 * @param delimiter Chaîne de caractères utilisée comme séparateur.
 
 * @return 0 si la tokenisation est réussie, 1 si trop de tokens, 2 si erreur de mémoire.
 */
int tokenizer(char *line, char *tokens[], int *nb_tokens, const char *delimiter);


/**
 * Libère la mémoire allouée pour les tokens extraits et réinitialise leur compteur.
 *
 * @param tokens Tableau contenant les tokens à nettoyer.
 * @param nb_tokens Pointeur vers le nombre actuel de tokens (sera mis à zéro).
 */
void cleanup_tokens(char *tokens[], int *nb_tokens);

#endif

