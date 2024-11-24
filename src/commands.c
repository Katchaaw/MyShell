#include "main.h"

int execute_command(const char *cmd, const char *file) {
    // Buffer pour stocker la commande modifiée
    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);

    // Remplacement de $F par le chemin du fichier
    char *pos = strstr(command, "$F");
    if (pos != NULL) {
        char new_cmd[1024];
        snprintf(new_cmd, pos - command + 1, "%s", command); // Partie avant $F
        snprintf(new_cmd + (pos - command), sizeof(new_cmd) - (pos - command), "%s%s", file, pos + 2); // Partie après $F
        strncpy(command, new_cmd, sizeof(command) - 1); // Mettre à jour la commande
        command[sizeof(command) - 1] = '\0'; // Sécurité
    }

    // Analyse et exécution de la commande comme si elle venait du prompt
    char *tokens[MAX_TOKENS];
    int nb_tokens = 0;

    // Copier la commande pour éviter de modifier la source
    char *command_copy = strdup(command);
    if (!command_copy) {
        perror("Erreur d'allocation mémoire");
        return 1;
    }

    // Tokenizer pour analyser la commande
    tokenizer(command_copy, tokens, &nb_tokens, " ");

    // Si des tokens ont été extraits, traiter la commande
    if (nb_tokens > 0) {
        char *cmd_name = tokens[0];
        char *arg = nb_tokens > 1 ? tokens[1] : NULL;

        // Gestion des commandes internes
        int last_return = 0;
        if (handle_interns(cmd_name, arg, &last_return) == 0) {
            cleanup_tokens(tokens, &nb_tokens); // Nettoyer les tokens
            free(command_copy);
            return last_return;
        }

        // Gestion des commandes externes
        int result = execute_external_command(cmd_name, tokens);

        cleanup_tokens(tokens, &nb_tokens); // Nettoyer les tokens
        free(command_copy);
        return result;
    }

    // Si aucun token, erreur
    free(command_copy);
    fprintf(stderr, "Erreur: commande vide après substitution\n");
    return 1;
}


int fsh_for(const char *rep, const char *cmd) {
    DIR *dir = opendir(rep);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignorer les fichiers cachés (qui commencent par '.')
        if (entry->d_name[0] == '.')
            continue;

        // Construire le chemin complet du fichier
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", rep, entry->d_name);

        // Remplace $F par le nom du fichier dans la commande
        execute_command(cmd, filepath);
    }

    closedir(dir);
    return 0;
}
