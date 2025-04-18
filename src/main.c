#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stddef.h>


int main() {
    char *line;                // Ligne lue depuis l'entrée utilisateur
    int last_return = 0;       // Code de retour de la dernière commande
    char *tokens[MAX_TOKENS];  // Tableau pour stocker les tokens
    int nb_tokens = 0;         // Nombre de tokens extraits

    rl_outstream = stderr;     // Redirige la sortie de readline vers stderr

    save_redirections();

    setup_signals();

    while (1) {
        reset_redirections();
        
        // Génère et affiche le prompt
        char *prompt = generate_prompt(last_return);

        line = readline(prompt);

        if (!line) { // Si readline retourne NULL (CTRL+D, CTRL+C...)
            break;
        }

        //Ignore les lignes vides
        if (line[0] == '\0' || strspn(line, " \t") == strlen(line)) {
            free(line);
            continue;
        }

        add_history(line); // Ajoute la commande à l'historique
        cleanup_tokens(tokens, &nb_tokens); // Reinitialisation des tokens

        // Duplication de la ligne pour éviter de modifier l'originale
        char *copy_line = strdup(line);
        if (!copy_line) {
            perror("Erreur de copie de la ligne");
            free(line);
            continue;
        }

        // Gestions des séquences
        if (strstr(copy_line,";") && !strstr(copy_line,"for") && !strstr(copy_line,"if")){
            last_return = execute_command(copy_line,NULL,NULL,'a');
            continue;
        }

        // Tokenisation de la ligne
        tokenizer(copy_line, tokens, &nb_tokens, " ");
        free(copy_line);

        // Gestion des pipelines
        if (check_pipe(tokens) == 1) {
            if (handle_pipe(tokens) == 0) {
                free(line);
                continue;
            }
        } 
        
        // Gestions des redirections
        else if (handle_redirections(tokens, &nb_tokens) == 1) {
            last_return = 1;
            free(line);
            continue;
        }

        // Modifie la ligne pour supprimer les redirections
        line[0] = '\0';  // Vide la ligne initiale
        for (int i = 0; i < nb_tokens; i++) {
            if (i > 0) {
                strcat(line, " ");  // Ajoute un espace entre les tokens
            }
            strcat(line, tokens[i]);  // Concatène chaque token à 'line'
        }

        // Gestion des if/else
        if (strcmp(tokens[0], "if") == 0) {
            if (handle_if_else(tokens, &nb_tokens, &last_return) == 0) {
                free(line);
                continue;
            }
        }

        // Séparation commande / argument
        char *command = strtok(line, " ");
        char *arg = strtok(NULL, " ");

        if (!command) {
            free(line);
            continue;
        }

        // Gestion des commandes internes
        if (handle_interns(command, arg, &last_return) == 0) {
            free(line);
            continue;
        }

        //Gestion des boucles for
        else if (strcmp(command, "for") == 0) {
            char concat[MAX_CMD_LENGTH] = {0};
            char *suite_cmd = strtok(NULL,"\0");
            strcat(concat,"for ");
            strcat(concat,arg);
            strcat(concat," ");
            strcat(concat,suite_cmd);
            last_return = execute_command(concat,NULL,NULL,'\0');
            free(line);
            continue;
        }

        // Gestion des commandes externes
        else { 
            // Préparation des arguments pour execvp
            char *argv[MAX_TOKENS + 1] = {command};
            for (int i = 1; i < nb_tokens; i++) {
                argv[i] = tokens[i];
            }
            argv[nb_tokens] = NULL;

            // Exécute la commande et met a jour la valeur de retour
            last_return = execute_external_command(command, argv);
        }
        free(line);
    }

    cleanup_tokens(tokens, &nb_tokens);
    close_saved_redirections();
    exit(last_return); 
    return 0;
}


