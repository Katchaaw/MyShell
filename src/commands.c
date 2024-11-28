#include "main.h"

int execute_external_command(char *cmd, char **args);

/**
 * Exécute une commande après avoir remplacé la variable `$F` par un fichier donné.
 *
 * Cette fonction traite la commande passée en argument de la manière suivante :
 * - Remplace toute occurrence de `$F` dans la commande par le chemin du fichier spécifié.
 * - Tokenise la commande pour extraire les arguments.
 * - Exécute la commande comme une commande interne si applicable, ou comme une commande externe sinon.
 *
 * @param cmd La commande à exécuter (peut contenir `$F` à remplacer).
 * @param file Le chemin du fichier à substituer à `$F`.
 * @return Le code de retour de la commande exécutée (0 si succès, autre valeur sinon).
 */

int execute_command(const char *cmd, const char *file) {
    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);

    char *pos = strstr(command, "$F");
    if (pos != NULL) {
        char new_cmd[1024];
        snprintf(new_cmd, pos - command + 1, "%s", command); 
        snprintf(new_cmd + (pos - command), sizeof(new_cmd) - (pos - command), "%s%s", file, pos + 2); 
        strncpy(command, new_cmd, sizeof(command) - 1); 
        command[sizeof(command) - 1] = '\0'; 
    }

    char *tokens[MAX_TOKENS];
    int nb_tokens = 0;

    char *command_copy = strdup(command);
    if (!command_copy) {
        perror("Erreur d'allocation mémoire");
        return 1;
    }

    tokenizer(command_copy, tokens, &nb_tokens, " ");

    if (nb_tokens > 0) {
        char *cmd_name = tokens[0];
        char *arg = nb_tokens > 1 ? tokens[1] : NULL;

        int last_return = 0;
        if (handle_interns(cmd_name, arg, &last_return) == 0) {
            cleanup_tokens(tokens, &nb_tokens); 
            free(command_copy);
            return last_return;
        }

        int result = execute_external_command(cmd_name, tokens);

        cleanup_tokens(tokens, &nb_tokens); 
        free(command_copy);
        return result;
    }

    free(command_copy);
    fprintf(stderr, "Erreur: commande vide après substitution\n");
    return 1;
}

/**
 * Exécute une commande pour chaque fichier (sauf les fichiers cachés) dans un répertoire donné.
 *
 * Cette fonction ouvre le répertoire spécifié, parcourt ses fichiers, et pour chaque fichier (à l'exception de ceux dont le nom commence par un point),
 * elle exécute la commande donnée en lui passant le chemin du fichier comme argument.
 *
 * @param rep Le chemin du répertoire à parcourir.
 * @param cmd La commande à exécuter pour chaque fichier.
 * @return 0 si la commande a été exécutée pour tous les fichiers, 1 en cas d'erreur (ex. répertoire introuvable).
 */

int fsh_for(const char *rep, const char *cmd) {
    DIR *dir = opendir(rep);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", rep, entry->d_name);

        execute_command(cmd, filepath);
    }

    closedir(dir);
    return 0;
}
