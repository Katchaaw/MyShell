#include "main.h"

int fsh_for(const char *rep, const char *cmd) {
    //printf("DEBUG: Début de la boucle 'for' avec répertoire : %s et commande : %s\n", rep, cmd);
    
    // Ouverture du répertoire
    DIR *dir = opendir(rep);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    // Parcours des fichiers du répertoire.
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.'){
            //printf("DEBUG: Fichier ignoré : %s (fichier caché ou spécial)\n", entry->d_name);
            continue;
        }

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", rep, entry->d_name);
        //printf("DEBUG: Fichier trouvé : %s\n", filepath);

        // Exécute la commande pour le fichier courant
        //printf("DEBUG: Exécution de la commande pour le fichier : %s\n", filepath);
        execute_command(cmd, filepath);
        //printf("DEBUG: Résultat de la commande pour %s : %d\n", filepath, result);
    }

    closedir(dir);
    //printf("DEBUG: Fin de la boucle 'for' pour le répertoire : %s\n", rep);
    return 0;
}


int handle_for(char *arg, int *last_return) {
    //printf("DEBUG: Gestion de la boucle 'for' avec argument : %s\n", arg);

    char *var = arg;              // La variable F
    char *in = strtok(NULL, " "); // Le mot-clé "in"
    char *rep = strtok(NULL, " "); // Le répertoire

    //printf("DEBUG: Variable : %s, Mot-clé : %s, Répertoire : %s\n", var, in, rep);

    if (var && in && rep && strcmp(in, "in") == 0) {
        char *cmd_start = strtok(NULL, "{");
        if (cmd_start != NULL) {
            cmd_start++; // Décaler pour ignorer le '{'
            char *cmd_end = strchr(cmd_start, '}');
            if (cmd_end != NULL) {
                *cmd_end = '\0'; 

                while (*cmd_start == ' ' || *cmd_start == '\t') {
                    cmd_start++;
                }

                //printf("DEBUG: Commande extraite pour la boucle : %s\n", cmd_start);

                if (strlen(var) == 1) {
                    *last_return = fsh_for(rep, cmd_start);
                } else {
                    fprintf(stderr, "Syntaxe incorrecte: for F in REP { CMD }\n");
                    *last_return = 1;
                }
            } else {
                fprintf(stderr, "Erreur: '}' manquant\n");
                *last_return = 1;
            }
        } else {
            fprintf(stderr, "Erreur: '{' manquant\n");
            *last_return = 1;
        }
    } else {
        fprintf(stderr, "Erreur: syntaxe incorrecte, la commande doit être : for F in REP { CMD }\n");
        *last_return = 1;
    }

    //printf("DEBUG: Fin de la gestion de la boucle 'for'\n");
    return 0;
}
