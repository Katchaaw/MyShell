#include "main.h"
#include <string.h>
#include <stdlib.h>

int tokenizer(char *line, char *tokens[], int *nb_tokens, const char *delimiter) {
    char *token;

    // Vérification des arguments
    if (line == NULL || tokens == NULL || nb_tokens == NULL || delimiter == NULL) {
        return 1; // Retourne une erreur pour arguments invalides
    }

    *nb_tokens = 0; // Réinitialise le compteur de tokens

    // Initialisation du token avec la première occurrence
    token = strtok(line, delimiter);

    // Continue d'extraire les tokens jusqu'à ce qu'il n'y en ait plus.
    while (token != NULL) {
        // Trop de tokens, retourne une erreur.
        if (*nb_tokens >= MAX_TOKENS) {
            cleanup_tokens(tokens, nb_tokens); // Libère les tokens déjà alloués
            return 11; // Retourner une erreur (trop de tokens)
        }

        // Duplique le token extrait dans le tableau de tokens.
        tokens[*nb_tokens] = strdup(token);
        if (tokens[*nb_tokens] == NULL) {
            // Erreur lors de l'allocation mémoire
            cleanup_tokens(tokens, nb_tokens); // Libère les tokens déjà alloués
            return 2;
        }

        (*nb_tokens)++;  // Incrémente le compteur de tokens
        token = strtok(NULL, delimiter);  // Récupère le token suivant.
    }

    return 0; // Succès
}

void cleanup_tokens(char *tokens[], int *nb_tokens) {
    if (tokens == NULL || nb_tokens == NULL) return;

    // Libère chaque token dupliqué.
    for (int i = 0; i < *nb_tokens; i++) {
        free(tokens[i]);
        tokens[i] = NULL; // Sécurise le pointeur
    }

    // Réinitialise le compteur de tokens.
    *nb_tokens = 0;
}
