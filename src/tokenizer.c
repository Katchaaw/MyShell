#include "main.h"

// Fonction de nettoyage de la mémoire allouée pour les tokens
void cleanup_tokens(char *tokens[], int *nb_tokens) {
    // Libérer chaque token dupliqué
    for (int i = 0; i < *nb_tokens; i++) {
        free(tokens[i]);  // Libérer chaque token dupliqué
    }

    // Réinitialiser le compteur de tokens
    *nb_tokens = 0;  // Réinitialiser le compteur de tokens
}

int tokenizer(char *line, char *tokens[], int *nb_tokens, const char *delimiter) {
    char *token;

    // Initialisation du token avec la première occurrence
    token = strtok(line, delimiter);
    
    // Parcourir la ligne et extraire les tokens
    while (token != NULL) {
        // Si le nombre de tokens atteint la limite, on retourne une erreur
        if (*nb_tokens >= MAX_TOKENS) {
            return 11;  // Retourner 1 en cas d'erreur (trop de tokens)
        }

        tokens[*nb_tokens] = strdup(token);  // Dupliquer le token et l'ajouter au tableau
        if (tokens[*nb_tokens] == NULL) {
            // Erreur lors de l'allocation mémoire
            return 2;
        }

        (*nb_tokens)++;  // Incrémenter le compteur
        token = strtok(NULL, delimiter);  // Obtenir le token suivant
    }

    // Retourner 0 en cas de succès
    return 0;
}
