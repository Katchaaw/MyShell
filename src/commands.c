#include "main.h"

// Fonction pour exécuter une commande avec un fichier
int execute_command(const char *cmd, const char *file) {
    char command[1024];
    snprintf(command, sizeof(command), "%s", cmd);
    
    // Remplace $F par le nom du fichier
    char *pos = strstr(command, "$F");
    if (pos != NULL) {
        // Calculer la longueur de la commande
        size_t cmd_len = strlen(command);
        size_t file_len = strlen(file);
        
        // Créer un nouveau buffer pour la commande modifiée
        char new_cmd[1024];
        snprintf(new_cmd, pos - command + 1, "%s", command);
        snprintf(new_cmd + (pos - command), sizeof(new_cmd) - (pos - command), "%s%s", file, pos + 2);
        
        // Exécuter la commande modifiée
        system(new_cmd);
    } else {
        system(command);
    }
    return 0;
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




















