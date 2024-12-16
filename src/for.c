#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include "for.h"

int fsh_for(const char *rep, const char *cmd); // Prototypage pour exécuter une boucle "for"

int handle_for(char *arg, int *last_return) {
    // Gestion des boucles for
    char *var = arg;              // La variable F
    char *in = strtok(NULL, " "); // Le mot-clé "in"
    char *rep = strtok(NULL, " "); // Le répertoire

    if (var && in && rep && strcmp(in, "in") == 0) {
        // Utilisation de strtok pour découper la partie de la commande entre '{' et '}'
        char *cmd_start = strtok(NULL, "{");
        if (cmd_start != NULL) {
            cmd_start++; // Décaler pour ignorer le '{'

            // Chercher la fermeture avec '}' et la remplacer par '\0' pour terminer la commande
            char *cmd_end = strchr(cmd_start, '}');
            if (cmd_end != NULL) {
                *cmd_end = '\0'; 

                // Nettoyer cmd_start pour enlever les espaces superflus
                while (*cmd_start == ' ' || *cmd_start == '\t') {
                    cmd_start++;
                }

                // Vérifie la syntaxe de la commande (variable, mot-clé "in", répertoire)
                if (strlen(var) == 1) {
                    // Exécute la boucle pour chaque fichier
                    *last_return = fsh_for(rep, cmd_start);
                } else {
                    fprintf(stderr, "Syntaxe incorrecte: for F in REP { CMD }\n");
                    *last_return = 1;
                }
            } else {
                fprintf(stderr, "Erreur: '}' manquant\n");
                *last_return = 1;
            }
        } else {
            fprintf(stderr, "Erreur: '{' manquant\n");
            *last_return = 1;
        }
    } else {
        fprintf(stderr, "Erreur: syntaxe incorrecte, la commande doit être : for F in REP { CMD }\n");
        *last_return = 1;
    }

    return 0; // Retourne 0 pour indiquer que la boucle "for" a été gérée
}
