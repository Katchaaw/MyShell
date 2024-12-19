#include "main.h"

// Permet de ne pas avoir le warning [-Wimplicit-function-declaration]
int execute_external_command(char *cmd, char **args);

void get_directory_from_file(const char *file, char *directory) {
    // Copier le chemin complet du fichier dans une nouvelle chaîne pour manipuler
    strcpy(directory, file);
    
    // Trouver le dernier '/' dans le chemin
    char *last_slash = strrchr(directory, '/');
    
    // Si un '/' est trouvé, couper la chaîne avant celui-ci
    if (last_slash != NULL) {
        *last_slash = '\0';  // Terminer la chaîne juste avant le dernier '/'
    }
}

int execute_command(const char *cmd, const char *file) {
    //printf("DEBUG: Commande initiale : %s, Fichier : %s\n", cmd, file);

    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);

    char directory[1024];  // Pour stocker le répertoire extrait
    
    // Extraire le répertoire à partir du chemin complet du fichier
    get_directory_from_file(file, directory);

    // Remplacement de toutes les occurrences de $F dans la commande par le chemin du fichier.
    char *pos = command;
    while ((pos = strstr(pos, "$F")) != NULL){ 
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


    
    char *pos0 = command;
    while ((pos0 = strstr(pos0, "$D")) != NULL){ 
        // Créer la nouvelle commande avec la substitution
        char new_cmd0[1024];
        // Copier tout avant $F
        int len_before0 = pos0 - command;
        snprintf(new_cmd0, len_before0 + 1, "%s", command);
        
        // Ajouter le fichier
            snprintf(new_cmd0 + len_before0, sizeof(new_cmd0) - len_before0, "%s", directory);
      
        // Ajouter la partie après $D
        snprintf(new_cmd0 + len_before0 + strlen(file), sizeof(new_cmd0) - (len_before0 + strlen(directory)), "%s", pos0 + 2);

        // Copier la nouvelle commande dans la variable d'origine
        strncpy(command, new_cmd0, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';

        // Avancer pour remplacer les autres occurrences
        pos0 = command + len_before0 + strlen(directory);
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
    //printf("commande : |%s|\n",command_copy);
    // Exécution de/des commande(s)
    if (nb_tokens > 0) {
        char *cmd_name = tokens[0];
        char *arg = nb_tokens > 1 ? tokens[1] : NULL;

        //printf("DEBUG: Commande à exécuter : %s, Premier argument : %s\n", cmd_name, arg ? arg : "NULL");
        //printf("cmdname : |%s|",cmd_name);
        // Commande interne
        int last_return = 0;
        if (handle_interns(cmd_name, arg, &last_return) == 0) {
            //printf("DEBUG: Commande à exécuter : %s, Premier argument : %s\n", cmd_name, arg ? arg : "NULL");
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
   
            return last_return;
        }
        else if (strcmp(cmd_name, "for") == 0) {
            char *ccmd = strtok(command," ");
            char *arg = strtok(NULL," ");
            handle_for(arg, &last_return);
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
