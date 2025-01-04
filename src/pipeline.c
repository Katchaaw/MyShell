#include "pipeline.h"
#include "redirections.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/wait.h>

#define MAX_CMDS 100
#define MAX_ARGS 100

int handle_pipe(char **tokens) {
    int pipes[MAX_CMDS - 1][2];         // Tableau pour stocker les pipes
    int cmd_count = 0;                  // Compteur de commandes
    char *commands[MAX_CMDS][MAX_ARGS]; // Commandes séparées en tokens

    int arg_index = 0;
    // Séparer les tokens en commandes
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            commands[cmd_count][arg_index] = NULL; // Terminer la commande courante
            cmd_count++;
            arg_index = 0;
        } else {
            commands[cmd_count][arg_index++] = tokens[i];
        }
    }
    commands[cmd_count][arg_index] = NULL; // Terminer la dernière commande
    cmd_count++;

    save_redirections();

    // Créer les pipes nécessaires
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    // Exécuter chaque commande
    for (int i = 0; i < cmd_count; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Dans le processus fils

            // Redirection de l'entrée si ce n'est pas la première commande
            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    perror("dup2 pour l'entrée");
                    exit(1);
                }
            }

            // Redirection de la sortie si ce n'est pas la dernière commande
            if (i < cmd_count - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    perror("dup2 pour la sortie");
                    exit(1);
                }
            }

            // Fermer tous les descripteurs de pipe inutilisés
            for (int j = 0; j < cmd_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Gérer les redirections
            int num_tokens = 0;
            while (commands[i][num_tokens] != NULL) {
                num_tokens++;
            }

            if (handle_redirections(commands[i], &num_tokens) != 0) {
                perror("Erreur lors des redirections");
                return 1;
            }

            // Exécuter la commande
            if (execvp(commands[i][0], commands[i]) == -1) {
                perror("execvp");
                return 1;
            }
            reset_redirections();
        }
    }

    // Dans le processus parent : fermer tous les pipes inutilisés
    for (int i = 0; i < cmd_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Attendre tous les processus fils
    for (int i = 0; i < cmd_count; i++) {
        wait(NULL);
    }

    return 0;
}

int check_pipe(char **tokens) {
    int i = 0;
    int is_previous_pipe = 0;
    int pipe_found = 0;  // Pour savoir s'il y a des pipes dans la commande.

    while (tokens[i] != NULL) {
        // Vérifier si un pipe est trouvé
        if (strcmp(tokens[i], "|") == 0) {
            // Vérifier que ce n'est pas le premier ou le dernier élément
            if (i == 0 || tokens[i + 1] == NULL) {
                perror("Erreur : pipe en début ou fin de commande\n");
                return 0; // Erreur, pipe en début ou fin
            }

            // Vérifier qu'il n'y a pas de pipes successifs
            if (is_previous_pipe) {
                perror("Erreur : pipe successif trouvé\n");
                return 0; // Erreur, deux pipes successifs
            }

            pipe_found = 1;  // On a trouvé au moins un pipe valide
            is_previous_pipe = 1;  // On marque qu'un pipe a été rencontré
        } else {
            is_previous_pipe = 0; // Ce n'est pas un pipe, on peut avoir un autre pipe ensuite
        }

        i++;
    }

    return pipe_found; // Retourne 1 si un pipeline valide est trouvé, sinon 0
}
