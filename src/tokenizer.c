#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int tokenizer(char *line, char *tokens[], int *nb_tokens, const char *delimiter) {
    char *token;

    // Initialisation du token avec la première occurrence
    token = strtok(line, delimiter);
    
    // Continue d'extraire les tokens jusqu'à ce qu'il n'y en ait plus.
    while (token != NULL) {
        if (*nb_tokens >= MAX_TOKENS) {
            return 1; 
        }

        // Duplique le token extrait dans le tableau de tokens.
        tokens[*nb_tokens] = strdup(token);
        if (tokens[*nb_tokens] == NULL) {
            return 2;
        }

        (*nb_tokens)++;  // Incrémente le compteur de tokens
        token = strtok(NULL, delimiter);  // Récupère le token suivant.
    }
    
    tokens[*nb_tokens] = NULL;
    return 0;
}


void cleanup_tokens(char *tokens[], int *nb_tokens) {
    for (int i = 0; i < *nb_tokens; i++) {
        free(tokens[i]); 
    }
    *nb_tokens = 0;  
}
