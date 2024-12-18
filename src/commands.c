#include "main.h"

// Permet de ne pas avoir le warning [-Wimplicit-function-declaration]
int execute_external_command(char *cmd, char **args);


int execute_command(const char *cmd, const char *file) {
    //printf("DEBUG: Commande initiale : %s, Fichier : %s\n", cmd, file);

    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);

    // Remplacement de toutes les occurrences de $F dans la commande par le chemin du fichier.
    char *pos = command;
    while ((pos = strstr(pos, "$F")) != NULL) {
        // Créer la nouvelle commande avec la substitution
        char new_cmd[1024];
        // Copier tout avant $F
        int len_before = pos - command;
        snprintf(new_cmd, len_before + 1, "%s", command);
        
        // Ajouter le fichier
        snprintf(new_cmd + len_before, sizeof(new_cmd) - len_before, "%s", file);
        
        // Ajouter la partie après $F
        snprintf(new_cmd + len_before + strlen(file), sizeof(new_cmd) - (len_before + strlen(file)), "%s", pos + 2);

        // Copier la nouvelle commande dans la variable d'origine
        strncpy(command, new_cmd, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';

        // Avancer pour remplacer les autres occurrences
        pos = command + len_before + strlen(file);
    }

    //printf("\ncmd : %s ; file : %s \n",cmd,file);


    // Tokenisation de la commande.
    char *tokens[MAX_TOKENS];
    int nb_tokens = 0;

    // Copie de la commande pour la tokenisation.
    char *command_copy = strdup(command);
    if (!command_copy) {
        perror("Erreur d'allocation mémoire");
        return 1;
    }

    tokenizer(command_copy, tokens, &nb_tokens, " ");
    /*printf("DEBUG: Commande tokenisée : ");
    for (int i = 0; i < nb_tokens; i++) {
        printf("[%s] ", tokens[i]);
    }
    printf("\n");*/

    // Exécution de/des commande(s)
    if (nb_tokens > 0) {
        char *cmd_name = tokens[0];
        char *arg = nb_tokens > 1 ? tokens[1] : NULL;

        //printf("DEBUG: Commande à exécuter : %s, Premier argument : %s\n", cmd_name, arg ? arg : "NULL");

        // Commande interne
        int last_return = 0;
        if (handle_interns(cmd_name, arg, &last_return) == 0) {
            //printf("DEBUG: Commande à exécuter : %s, Premier argument : %s\n", cmd_name, arg ? arg : "NULL");
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
   
            return last_return;
        }

        // Commande externe
        int result = execute_external_command(cmd_name, tokens);
        //printf("DEBUG: Commande externe exécutée avec code retour : %d\n", result);

        cleanup_tokens(tokens, &nb_tokens); 
        free(command_copy);
        return result;
    }

    free(command_copy);
    fprintf(stderr, "Erreur: commande vide après substitution\n");
    return 1;
}
