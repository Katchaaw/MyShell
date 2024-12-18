#include "main.h"

int handle_if(char **tokens, int *nb_tokens, const char *file) {
    if (*nb_tokens < 2 || strcmp(tokens[0], "if") != 0) {
        return 0;  // Ce n'est pas une commande if, laissez-la être traitée normalement
    }
    int condition_start = -1, condition_end = -1;
    int if_block_start = -1, if_block_end = -1;
    int else_block_start = -1, else_block_end = -1;

    // Identifier les positions des différentes sections
    for (int i = 0; i < *nb_tokens; i++) {
        if (strcmp(tokens[i], "if") == 0) {
            condition_start = i + 1;
            if (tokens[i + 1] != NULL && strcmp(tokens[i + 1], "[") == 0 && condition_end == -1)
            condition_start = i + 2;
        } else if (strcmp(tokens[i], "]") == 0 && condition_start != -1 && condition_end == -1) {
            condition_end = i - 1;
        } else if (strcmp(tokens[i], "{") == 0) {
            if (if_block_start == -1) {
                if_block_start = i + 1;
                if (condition_end == -1) {
                    condition_end = i - 1;
                }
            } else if (if_block_end != -1) {
                else_block_start = i + 1;
            }
        } else if (strcmp(tokens[i], "}") == 0) {
            if (if_block_end == -1) {
                if_block_end = i - 1;
            } else if (else_block_start != -1 && else_block_end == -1) {
                else_block_end = i - 1;
            } 
        }
    }

   // Validation de la structure
    if (condition_start == -1) {
        return 0;
    } else if (condition_end == -1 || if_block_start == -1 || if_block_end == -1) {
        fprintf(stderr, "Erreur: structure if invalide\n");
        return 1;
    }

    // Construire et évaluer la condition
    char condition_cmd[1024] = {0};
    for (int i = condition_start; i <= condition_end; i++) {
        strcat(condition_cmd, tokens[i]);
        strcat(condition_cmd, " ");
    }

    int condition_result = evaluate_condition(tokens, condition_start, condition_end);

    if (condition_result == 0) {
        // Condition vraie : exécuter le bloc if
        if (execute_block(tokens, if_block_start, if_block_end, file) != 0) {
            fprintf(stderr, "Erreur: échec dans le bloc if\n");
            return 1;
        }
    } else if (else_block_start != -1 && else_block_end != -1) {
        // Condition fausse : exécuter le bloc else
        if (execute_block(tokens, else_block_start, else_block_end, file) != 0) {
            fprintf(stderr, "Erreur: échec dans le bloc else\n");
            return 1;
        }
    }

    // Nettoyer les tokens pour les commandes restantes
    for (int i = condition_start - 1; i <= (else_block_end != -1 ? else_block_end : if_block_end); i++) {
        tokens[i] = NULL;
    }

    *nb_tokens -= (else_block_end != -1 ? else_block_end : if_block_end) - (condition_start - 1) + 1;
    return 0;
}

int execute_block(char **tokens, int start, int end, const char *file) {
    char block_cmd[1024] = {0};
    for (int i = start; i <= end; i++) {
        strcat(block_cmd, tokens[i]);
        strcat(block_cmd, " ");
    }
    return execute_command(block_cmd, file);
}

int evaluate_condition(char **tokens, int start, int end) {
    if (end - start != 2) {
        fprintf(stderr, "Erreur: condition invalide\n");
        return 1;
    }

    char *left = tokens[start];
    char *op = tokens[start + 1];
    char *right = tokens[start + 2];

    int left_val = atoi(left);
    int right_val = atoi(right);

    if (strcmp(op, "-eq") == 0) {
        return left_val == right_val ? 0 : 1;
    } else if (strcmp(op, "-ne") == 0) {
        return left_val != right_val ? 0 : 1;
    } else if (strcmp(op, "-lt") == 0) {
        return left_val < right_val ? 0 : 1;
    } else if (strcmp(op, "-le") == 0) {
        return left_val <= right_val ? 0 : 1;
    } else if (strcmp(op, "-gt") == 0) {
        return left_val > right_val ? 0 : 1;
    } else if (strcmp(op, "-ge") == 0) {
        return left_val >= right_val ? 0 : 1;
    } else {
        fprintf(stderr, "Erreur: opérateur conditionnel inconnu '%s'\n", op);
        return 1;
    }
}