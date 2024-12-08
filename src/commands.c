#include "main.h"

// Permet de ne pas avoir le warning [-Wimplicit-function-declaration]
int execute_external_command(char *cmd, char **args);


int execute_command(const char *cmd, const char *file) {
    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);

    // Recherche de la variable $F dans la commande et remplacement par le chemin du fichier.
    char *pos = strstr(command, "$F");
    if (pos != NULL) {
        char new_cmd[1024];
        snprintf(new_cmd, pos - command + 1, "%s", command); 
        snprintf(new_cmd + (pos - command), sizeof(new_cmd) - (pos - command), "%s%s", file, pos + 2); 
        strncpy(command, new_cmd, sizeof(command) - 1); 
        command[sizeof(command) - 1] = '\0'; 
    }

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

    // Exécution de/des commande(s)
    if (nb_tokens > 0) {
        char *cmd_name = tokens[0];
        char *arg = nb_tokens > 1 ? tokens[1] : NULL;

        // Commande interne
        int last_return = 0;
        if (handle_interns(cmd_name, arg, &last_return) == 0) {
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
            return last_return;
        }

        // Commande externe
        int result = execute_external_command(cmd_name, tokens);

        cleanup_tokens(tokens, &nb_tokens); 
        free(command_copy);
        return result;
    }

    free(command_copy);
    fprintf(stderr, "Erreur: commande vide après substitution\n");
    return 1;
}

int fsh_for(const char *rep, const char *cmd) {
    // Ouverture du répertoire
    DIR *dir = opendir(rep);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    // Parcours des fichiers du répertoire.
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", rep, entry->d_name);

        // Exécute la commande pour le fichier courant.
        execute_command(cmd, filepath);
    }

    closedir(dir);
    return 0;
}
