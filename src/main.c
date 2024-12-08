#include "main.h"

int main() {
    char *line; // Ligne lue depuis l'entrée utilisateur
    int last_return = 0; // Code de retour de la dernière commande
    char *tokens[MAX_TOKENS];  // Tableau pour stocker les tokens
    int nb_tokens = 0;  // Nombre de tokens extraits

    rl_outstream = stderr; // Redirige la sortie de readline vers stderr

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
            // Variables pour stocker la syntaxe de la boucle
            char *var = arg; // La variable F
            char *in = strtok(NULL, " "); // Le mot-clé "in"
            char *rep = strtok(NULL, " "); // Le répertoire

            if (var && in && rep && strcmp(in, "in") == 0) {
                // Utilisation de strtok pour découper la partie de la commande entre '{' et '}'
                char *cmd_start = strtok(NULL, "{");
                if (cmd_start != NULL) {
                    cmd_start++; // Décaler pour ignorer le '{'

                    // Chercher la fermeture avec '}'et la remplacer par '\0' pour terminer la commande
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

        free(line);
    }

    cleanup_tokens(tokens, &nb_tokens); 
    exit(last_return); 
    return 0;
}

