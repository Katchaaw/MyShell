#include "main.h"
#include <sys/wait.h>
#include <unistd.h>

int execute_external_command(char *cmd, char **args) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1; 
    }

    if (pid == 0) {
        // Dans le processus fils, on exécute la commande
        if (execvp(cmd, args) == -1) { 
            perror("execvp");
            exit(1);
        }
    } else {
        // Processus père
        int status;
        waitpid(pid, &status, 0); // Attend que le fils se termine

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return 1; 
        }
    }

    return 0;
}



int main() {
    char *line; // Stock la ligne lue par l'utilisateur
    int last_return = 0; // Code de retour de la dernière commande
    char *tokens[MAX_TOKENS];  // Tableau pour stocker les tokens
    int nb_tokens = 0;  // Compteur de tokens

    rl_outstream = stderr; // Redirige la sortie de readline vers stderr

    while (1) {
        char *prompt = generate_prompt(last_return);

        line = readline(prompt);

        if (!line) { // Si readline retourne NULL (CTRL+D, CTRL+C...)
            break;
        }

        if (line[0] == '\0' || strspn(line, " \t") == strlen(line)) {
            free(line);
            continue;
        }



        add_history(line); // Ajoute la ligne à l'historique des commandes
        cleanup_tokens(tokens, &nb_tokens); // Reinitialisation des tokens

        // Copier la ligne lue avant l'appel à tokenizer
        char *copy_line = strdup(line);
        if (!copy_line) { // Vérifier l'échec de strdup
            perror("Erreur de copie de la ligne");
            free(line);
            continue;  // Continue avec la prochaine itération
        }

        // Appel de la fonction tokenizer pour remplir le tableau de tokens
        tokenizer(copy_line, tokens, &nb_tokens, " ");

        free(copy_line);

        // Sépare la ligne en commande et argument
        char *command = strtok(line, " ");
        char *arg = strtok(NULL, " ");

        if (command == NULL) { // Si aucune commande n'est saisie
            free(line);
            continue;
        }

        // Gestion des commandes internes
        if (handle_interns(command, arg, &last_return) == 0) {
            free(line);
            continue;;
        }

        //Gestion des boucles for
        if (strcmp(command, "for") == 0) {
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

        // Gestion des commandes externes
        else { 
            // Préparation des arguments pour execvp
            char *argv[MAX_TOKENS + 1] = {command};
            for (int i = 1; i < nb_tokens; i++) {
                argv[i] = tokens[i];
            }
            argv[nb_tokens] = NULL;

            // Appel à la fonction pour exécuter la commande externe
            last_return = execute_external_command(command, argv);
        }

        free(line);
    }

    return 0;
}

