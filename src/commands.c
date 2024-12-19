#include "main.h"
#define MAX_LENGTH 256

// Permet de ne pas avoir le warning [-Wimplicit-function-declaration]
int execute_external_command(char *cmd, char **args);

char *args_to_cmd(char **args) {
    if (args == NULL) return NULL;

    // Calculer la taille totale de la commande.
    size_t total_length = 0;
    for (int i = 0; args[i] != NULL; i++) {
        total_length += strlen(args[i])+1;
    }

    // Allouer la mémoire pour la commande finale.
    char *cmd = malloc(total_length + 1);
    if (cmd == NULL) {
        perror("malloc failed");
        return NULL;
    }

    // Concaténer les chaînes dans cmd
    cmd[0] = '\0';
    for (int i = 0; args[i] != NULL; i++) {
        strcat(cmd, args[i]);
        strcat(cmd, " ");
    }
    return cmd;
}

int execute_from_if(char **args){
    char *new_cmd = args_to_cmd(args);  // Transforme les arguments en une seule commande.
    int result = execute_command(new_cmd, NULL, NULL, 0);  
    free(new_cmd);  
    return result;
}


void replaceVariable(char *command, char variable, const char *replacement) {
    char buffer[MAX_LENGTH];  // Buffer temporaire pour la commande modifiée.
    char *pos = command;
    char *match;

    buffer[0] = '\0';

    while ((match = strstr(pos, "$")) != NULL) {
        if (*(match + 1) == variable) {
            strncat(buffer, pos, match - pos);  // Ajouter la partie avant `$variable`.
            strncat(buffer, replacement, strlen(replacement));  // Ajouter la valeur de remplacement.
            pos = match + 2;  // Passer `$variable`.
        } else {
            strncat(buffer, pos, match - pos + 1);  // Ajouter jusqu'au caractère suivant '$'.
            pos = match + 1;
        }
    }
    strncat(buffer, pos, MAX_LENGTH - strlen(buffer) - 1);  // Ajouter le reste de la commande.
    strncpy(command, buffer, MAX_LENGTH - 1);  
    command[MAX_LENGTH - 1] = '\0';
}


int execute_command(const char *cmd, const char *file, const char *directory,char variable) {

    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);

    
    char *command_cop = strdup(command);
    char *res = command_cop;

    // Gestion des commandes multiples - séparées par ';'.
    if ((strstr(res, ";")) != NULL){
        if ((strstr(res, "for")) == NULL || (strstr(res, "; for")) != NULL){
        char *cmd1 = strtok(command_cop, ";");
        char *cmd2 = strtok(NULL, "\0");

        execute_command(cmd1, file, directory, variable);
        int result = execute_command(cmd2, file, directory, variable);
            
        free(command_cop);

        return result;
        }
    }
    free(command_cop);

    // Remplacement des variables dans la commande.
    if (variable !=NULL){
        replaceVariable(command,variable,file);
    }

    // Tokenisation de la commande.
    char *tokens[MAX_TOKENS];
    int nb_tokens = 0;
    char *command_copy = strdup(command);
    if (!command_copy) {
        perror("Erreur d'allocation mémoire");
        return 1;
    }

    tokenizer(command_copy, tokens, &nb_tokens, " ");

    // Exécution de/des commande(s)
    if (nb_tokens > 0) {
        char *cmd_name = tokens[0];
        char *arg = nb_tokens > 1 ? tokens[1] : NULL;

        int last_return = 0;

        // Gestion des commandes internes
        if (handle_interns(cmd_name, arg, &last_return) == 0) {
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
            return last_return;
        }

        // Gestion des boucles 'for'
        else if (strcmp(cmd_name, "for") == 0) {
            strtok(command," ");
            char *arg = strtok(NULL," ");
            handle_for(arg, &last_return);
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
            return last_return;
        }

        // Gestion des 'if'
        else if (strcmp(cmd_name, "if") == 0) {
            handle_if_else(tokens, &nb_tokens,&last_return);
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
            return last_return;
        }
        
        // Gestion des commandes externes
        int result = execute_external_command(cmd_name, tokens);
       
        cleanup_tokens(tokens, &nb_tokens); 
        free(command_copy);
        return result;
    }

    free(command_copy);
    fprintf(stderr, "Erreur: commande vide après substitution\n");
    return 1;
}
