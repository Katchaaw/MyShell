#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "commands.h"

#define PROMPT_SIZE 32




int main() {
    char *line; // Stock la ligne lue par l'utilisateur
    int last_return = 0; // Code de retour de la dernière commande

    rl_outstream = stderr; // Redirige la sortie de readline vers stderr

    while (1) {
        char prompt[PROMPT_SIZE];
        // Prépare le prompt avec la bonne couleur selon le dernier code de retour
        snprintf(prompt, PROMPT_SIZE, "\001\033[%sm\002[%d]\001\033[00m\002$ ",
                 last_return == 0 ? "32" : "91", last_return);

        line = readline(prompt);

        if (!line) { // Si readline retourne NULL (CTRL+D, CTRL+C...)
            printf("exit\n");
            break;
        }

        add_history(line); // Ajoute la ligne à l'historique des commandes

        // Sépare la ligne en commande et argument
        char *command = strtok(line, " ");
        char *arg = strtok(NULL, " ");

        if (command == NULL) { // Si aucune commande n'est saisie
            free(line);
            continue;
        }


        // Gestion des commandes internes
        if (strcmp(command, "pwd") == 0) {
            last_return = fsh_pwd(); // pwd
        }
        else if (strcmp(command, "cd") == 0) {
            last_return = fsh_cd(arg); // cd
        }
        else if (strcmp(command, "exit") == 0) {
            int exit_code = arg ? atoi(arg) : last_return;
            free(line);
            fsh_exit(exit_code); // exit
        }
        else if (strcmp(command, "ftype") == 0) {
            if (arg) {
                last_return = fsh_ftype(arg); // ftype
            } else {
                fprintf(stderr, "ftype: argument requis\n");
                last_return = 1;
            }
        }

else if (strcmp(command, "for") == 0) {
    // Variables pour stocker la syntaxe de la boucle
    char *var = arg; // La variable F
    char *in = strtok(NULL, " "); // Le mot-clé "in"
    char *rep = strtok(NULL, " "); // Le répertoire

    if (var && in && rep && strcmp(in, "in") == 0) {
        // Utilisation de strtok pour découper la partie de la commande entre { et }
        char *cmd_start = strtok(NULL, "{"); // Trouver la première partie avant '{'
        if (cmd_start != NULL) {
            // Chercher le reste de la commande après la fermeture de la commande '}'.
            cmd_start++; // Décaler pour ignorer le '{'

            // Chercher la fermeture avec '}'
            char *cmd_end = strchr(cmd_start, '}');
            if (cmd_end != NULL) {
                *cmd_end = '\0'; // Remplace '}' par '\0' pour terminer la commande

                // Nettoyer cmd_start pour enlever les espaces superflus
                while (*cmd_start == ' ' || *cmd_start == '\t') {
                    cmd_start++; // Ignorer les espaces au début de la commande
                }

                // Vérifie la syntaxe de la commande (variable, mot-clé "in", répertoire)
                if (strlen(var) == 1) {
                    // Exécute la boucle pour chaque fichier
                    last_return = fsh_for(rep, cmd_start);
                } else {
                    fprintf(stderr, "Syntaxe incorrecte: for F in REP { CMD }\n");
                    last_return = 1;
                }
            } else {
                fprintf(stderr, "Erreur: '}' manquant\n");
                last_return = 1;
            }
        } else {
            fprintf(stderr, "Erreur: '{' manquant\n");
            last_return = 1;
        }
    } else {
        fprintf(stderr, "Erreur: syntaxe incorrecte, la commande doit être : for F in REP { CMD }\n");
        last_return = 1;
    }
}







        else { // Gestion des commandes externes
            pid_t pid = fork(); // Crée un nouveau processus
            if (pid == -1) {
                perror("Erreur fork");
                last_return = 1;
            }

            else if (pid == 0) { // Processus enfant
                if (execlp(command, command, arg, (char *)NULL) == -1) {
                    perror("Erreur exec");
                    exit(1);
                }
            }

            else { // Processus parent
                int status;
                waitpid(pid, &status, 0); // Attend la fin du processus enfant
                last_return = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
            }
        }

        free(line);
    }

    return 0;
}
