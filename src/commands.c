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
int maxi (int a, int b){
    if (a> b){
        return a;
    }
    return b;
}

char *args_to_cmd(char **args) {
    // Vérifier si le tableau est valide
    if (args == NULL) return NULL;

    // Calculer la taille totale nécessaire pour la chaîne finale
    size_t total_length = 0;
    for (int i = 0; args[i] != NULL; i++) {
        total_length += strlen(args[i])+1;
    }

    // Allouer la mémoire pour cmd (+1 pour le caractère de fin '\0')
    char *cmd = malloc(total_length + 1);
    if (cmd == NULL) {
        perror("malloc failed");
        return NULL;
    }

    // Concaténer les chaînes dans cmd
    cmd[0] = '\0'; // Initialiser cmd comme une chaîne vide
    for (int i = 0; args[i] != NULL; i++) {
        strcat(cmd, args[i]);
        strcat(cmd, " ");
    }
    //printf("cmdd flattenn %s\n",cmd);
    return cmd; // Retourner la chaîne résultante
}


int execute_from_if(char **args){
    char *new_cmd = args_to_cmd(args);
    return execute_command(new_cmd,NULL,NULL);
}

int execute_command(const char *cmd, const char *file, const char *directory) {
    //printf("DEBUG: Commande initiale : %s, Fichier : %s\n", cmd, file);

    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);
    char *command_cop = strdup(command);
    char *res = command_cop;
    


    if ((strstr(res, ";")) != NULL){
        if ((strstr(res, "for")) == NULL || (strstr(res, "; for")) != NULL){
        char *cmd1 = strtok(command_cop, ";");
        char *cmd2 = strtok(NULL, "\0");

        
        //if (strstr(cmd1, "true")){return execute_command(cmd2, file, directory);}
        //else if (strstr(cmd1, "false")){return 1 + execute_command(cmd2, file, directory);}
        //else{
        //printf("valMAX :%d\n", maxi(execute_command(cmd1, file, directory),execute_command(cmd2, file, directory)));
        //printf("val2 :%d\n", execute_command(cmd2, file, directory));
        //return maxi(execute_command(cmd1, file, directory),execute_command(cmd2, file, directory));
        execute_command(cmd1, file, directory);
        return execute_command(cmd2, file, directory);
        }
    }

    


    //char directory[1024];  // Pour stocker le répertoire extrait
    
    // Extraire le répertoire à partir du chemin complet du fichier
    //get_directory_from_file(file, directory);
    // Remplacement de toutes les occurrences de $F dans la commande par le chemin du fichier.
    char *pos = command;
while ((pos = strchr(pos, '$')) != NULL) {
    // Vérifier qu'il y a au moins un caractère après '$'
    if (*(pos + 1) != '\0') {
        char *test = command;
        if (strstr(test, "for") == NULL) {
            // Créer la nouvelle commande avec la substitution
            char new_cmd[1024];
            // Copier tout avant le '$'
            int len_before = pos - command;
            snprintf(new_cmd, len_before + 1, "%s", command);
            
            // Ajouter le fichier (ou autre valeur de substitution)
            snprintf(new_cmd + len_before, sizeof(new_cmd) - len_before, "%s", file);
            
            // Ajouter la partie après '$X' (où X est le caractère suivant '$')
            snprintf(new_cmd + len_before + strlen(file), sizeof(new_cmd) - (len_before + strlen(file)), "%s", pos + 2);
            
            // Copier la nouvelle commande dans la variable d'origine
            strncpy(command, new_cmd, sizeof(command) - 1);
            command[sizeof(command) - 1] = '\0';
            
            // Avancer pour remplacer les autres occurrences
            pos = command + len_before + strlen(file);
        } else {
            break;
        }
    } else {
        break; // Aucun caractère après '$', fin de traitement
    }
}


    
    char *pos0 = command;
    while ((pos0 = strstr(pos0, "$D")) != NULL){ 
        char *test0 = command;
        if (strstr(test0, "for") == NULL || strstr(test0, "in $D") != NULL){
        // Créer la nouvelle commande avec la substitution
        char new_cmd0[1024];
        // Copier tout avant $D
        int len_before0 = pos0 - command;
        snprintf(new_cmd0, len_before0 + 1, "%s", command);
        
        // Ajouter le fichier
            snprintf(new_cmd0 + len_before0, sizeof(new_cmd0) - len_before0, "%s", directory);
      
        // Ajouter la partie après $D
        snprintf(new_cmd0 + len_before0 + strlen(directory), sizeof(new_cmd0) - (len_before0 + strlen(directory)), "%s", pos0 + 2);

        // Copier la nouvelle commande dans la variable d'origine
        strncpy(command, new_cmd0, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';

        // Avancer pour remplacer les autres occurrences
        pos0 = command + len_before0 + strlen(directory);
        }
        else {
            break;}
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
        else if (strcmp(cmd_name, "if") == 0) {
            //printf("teeeeeeeeeeeeeeest\n");
            
            handle_if_else(tokens, &nb_tokens,&last_return);
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
