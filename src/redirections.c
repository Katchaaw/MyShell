#include "redirections.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


static int saved_stdin, saved_stdout, saved_stderr;

int handle_redirections(char **tokens, int *nb_tokens) {

    // Parcours des tokens pour identifier les redirections.
    for (int i = 0; i < *nb_tokens; i++) {
        int fd = -1; // Descripteur de fichier
        int target_fd = -1; // Descripteur de fichier cible : entrée, sortie ou erreur
        int flags = 0; // Drapeaux pour l'ouverture des fichiers

        // Identifie le type de redirection
        if (strcmp(tokens[i], "<") == 0) {
            target_fd = STDIN_FILENO; // Redirection de l'entrée standard
            flags = O_RDONLY; // Ouverture en lecture seule
        } else if (strcmp(tokens[i], ">") == 0) {
            target_fd = STDOUT_FILENO; // Redirection de sortie
            flags = O_WRONLY | O_CREAT | O_EXCL; // Echec si fichier existe déjà
        } else if (strcmp(tokens[i], ">|") == 0) {
            target_fd = STDOUT_FILENO;
            flags = O_WRONLY | O_CREAT | O_TRUNC; // Ecraseement
        } else if (strcmp(tokens[i], ">>") == 0) {
            target_fd = STDOUT_FILENO;
            flags = O_WRONLY | O_CREAT | O_APPEND; // Concatenation
        } else if (strcmp(tokens[i], "2>") == 0) {
            target_fd = STDERR_FILENO;
            flags = O_WRONLY | O_CREAT | O_EXCL; // Echec si le fichier existe
        } else if (strcmp(tokens[i], "2>|") == 0) {
            target_fd = STDERR_FILENO;
            flags = O_WRONLY | O_CREAT | O_TRUNC; // Ecrasement
        } else if (strcmp(tokens[i], "2>>") == 0) {
            target_fd = STDERR_FILENO;
            flags = O_WRONLY | O_CREAT | O_APPEND; // Concatenation
        } else {
            continue;
        }

        // Vérifie qu'un fichier suit l'opérateur
        if (i + 1 >= *nb_tokens) {
            perror("Erreur: fichier manquant après l'opérateur\n");
            return 1;
        }

        // Ouvre le fichier avec les bons drapeaux
        char *filename = tokens[i + 1];
        fd = open(filename, flags, 0644);
        if (fd == -1) {
            perror("Erreur d'ouverture du fichier");
            close(fd);
            return 1;
        }

        // Redirige le descripteur de fichier cible vers le fichier ouvert
        if (dup2(fd, target_fd) == -1) {
            perror("Erreur lors de dup2");
            close(fd);
            return 1;
        }

        close(fd); //Ferme le descripteur de fichier

        // Libère la mémoire des éléments supprimés du tableau
        if (tokens[i]) {
            free(tokens[i]);
            tokens[i] = NULL;
        }
        if (tokens[i + 1]) {
            free(tokens[i + 1]);
            tokens[i + 1] = NULL;
        }

        // Supprime l'opérateur et le fichier des tokens
        for (int j = i; j + 2 < *nb_tokens; j++) {
            tokens[j] = tokens[j + 2];
        }
        *nb_tokens -= 2;
        tokens[*nb_tokens] = NULL;

        // Recommence à analyser depuis le début (si plusieurs redirections)
        i = -1;
    }
    return 0;
}

void save_redirections() {
    if ((saved_stdin = dup(STDIN_FILENO)) == -1 ||
        (saved_stdout = dup(STDOUT_FILENO)) == -1 ||
        (saved_stderr = dup(STDERR_FILENO)) == -1) {
        perror("Erreur lors de la sauvegarde des redirections");
        exit(1);
    }
}

void reset_redirections() {
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
}

void close_saved_redirections() {
    close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);
}

