#include "main.h"

int main() {
    char *line; // Ligne lue depuis l'entrée utilisateur
    int last_return = 0; // Code de retour de la dernière commande
    char *tokens[MAX_TOKENS];  // Tableau pour stocker les tokens
    int nb_tokens = 0;  // Nombre de tokens extraits

    rl_outstream = stderr; // Redirige la sortie de readline vers stderr

    save_redirections();

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

        // Tokenisation de la ligne
        tokenizer(copy_line, tokens, &nb_tokens, " ");
        free(copy_line);

        // Appel à la gestion des redirections
        if (handle_redirections(tokens, &nb_tokens) == 1) {
            last_return = 1;
            free(line);
            continue;
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
            continue;;
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

        //Gestion des boucles for
        if (strcmp(command, "for") == 0) {
            handle_for(arg, &last_return);
            free(line);
            continue;
        }
        
        free(line);
    }

    cleanup_tokens(tokens, &nb_tokens);
    close_saved_redirections();
    exit(last_return); 
    return 0;
}

