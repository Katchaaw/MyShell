#include "main.h"

int handle_if_else(char **tokens, int *nb_tokens, int *last_return) {
    // Vérifie que la commande commence par "if"
    if (strcmp(tokens[0], "if") != 0) {
        return 1; 
    }

    // Trouve le pipeline TEST/Prédicat
    int i = 1;
    char *test_cmd[MAX_TOKENS];
    int test_cmd_count = 0;

    // Parcours jusqu'à la première accolade '{'
    while (i < *nb_tokens && strcmp(tokens[i], "{") != 0) {
        test_cmd[test_cmd_count++] = tokens[i++];
    }

    if (i == *nb_tokens || strcmp(tokens[i], "{") != 0) {
        fprintf(stderr, "Erreur : 'if' sans bloc { ... }\n");
        return 1;
    }
    test_cmd[test_cmd_count] = NULL;

    // Exécute le pipeline TEST
    int test_result = execute_external_command(test_cmd[0], test_cmd);

    // Passe au bloc { CMD_1 }
    i++; // Ignore le '{'
    int cmd1_start = i;
    int brace_count = 1;

    // Parcours pour trouver la fin du premier bloc CMD_1
    while (i < *nb_tokens && brace_count > 0) {
        if (strcmp(tokens[i], "{") == 0) brace_count++;
        if (strcmp(tokens[i], "}") == 0) brace_count--;
        i++;
    }
    if (brace_count != 0) {
        fprintf(stderr, "Erreur : blocs { ... } mal équilibrés\n");
        return 1;
    }
    int cmd1_end = i - 1;

    // Si TEST/le prédicat réussi (retourne 0), on exécute le bloc CMD_1
    if (test_result == 0) {
        // Sépare les commandes dans le bloc CMD_1 en fonction des points-virgules
        char *cmd1[MAX_TOKENS];
        int cmd1_count = 0;
        for (int j = cmd1_start; j < cmd1_end; j++) {
            if (strcmp(tokens[j], ";") == 0) {
                // Exécute la commande courante quand on rencontre un point-virgule
                cmd1[cmd1_count] = NULL;
                if (cmd1_count > 0) {
                    *last_return = execute_external_command(cmd1[0], cmd1);
                }
                cmd1_count = 0; // Réinitialise pour la prochaine commande
            } else {
                cmd1[cmd1_count++] = tokens[j];
            }
        }

        // Exécute la dernière commande de CMD_1 si elle n'a pas été exécutée
        if (cmd1_count > 0) {
            cmd1[cmd1_count] = NULL;
            *last_return = execute_external_command(cmd1[0], cmd1);
        }
    } else {
        // Recherche du bloc `else { CMD_2 }`, s'il existe
        if (i < *nb_tokens - 1 && strcmp(tokens[i], "else") == 0) {
            i++; // Ignore `else`
            if (strcmp(tokens[i], "{") != 0) {
                fprintf(stderr, "Erreur : 'else' sans bloc { ... }\n");
                return 1;
            }
            i++; // Ignore le '{'
            int cmd2_start = i;
            brace_count = 1;

            // Trouve la fin du bloc CMD_2
            while (i < *nb_tokens && brace_count > 0) {
                if (strcmp(tokens[i], "{") == 0) brace_count++;
                if (strcmp(tokens[i], "}") == 0) brace_count--;
                i++;
            }
            if (brace_count != 0) {
                fprintf(stderr, "Erreur : blocs { ... } mal équilibrés\n");
                return 1;
            }
            int cmd2_end = i - 1;

            // Sépare les commandes dans le bloc CMD_2 en fonction des points-virgules
            char *cmd2[MAX_TOKENS];
            int cmd2_count = 0;
            for (int j = cmd2_start; j < cmd2_end; j++) {
                if (strcmp(tokens[j], ";") == 0) {
                    // Exécute la commande courante quand on rencontre un point-virgule
                    cmd2[cmd2_count] = NULL;
                    if (cmd2_count > 0) {
                        *last_return = execute_external_command(cmd2[0], cmd2);
                    }
                    cmd2_count = 0; // Réinitialise pour la prochaine commande
                } else {
                    cmd2[cmd2_count++] = tokens[j];
                }
            }

            // Exécute la dernière commande de CMD_2 si elle n'a pas été exécutée
            if (cmd2_count > 0) {
                cmd2[cmd2_count] = NULL;
                *last_return = execute_external_command(cmd2[0], cmd2);
            }
        } else {
            *last_return = 0; // Si pas de `else`, retourne 0
        }
    }

    return 0;
}