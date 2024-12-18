#include "main.h"

int fsh_for(const char *rep, const char *cmd,int opt_A, int opt_r,const char *opt_ext,int test) { //char opt_type
    //printf("DEBUG: Début de la boucle 'for' avec répertoire : |%s| et commande : |%s|\n", rep, cmd);
    printf("\noption -A : [%d]\noption -r : [%d]\noption -e : [%s]\n",opt_A,opt_r,opt_ext);
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
#define MAX_CMD_LENGTH 1024 // Taille maximale de la commande

int handle_for(char *arg, int *last_return) {
    int have_opt = 0;
    int opt_A =0;
    int opt_r =0;
    char *ext = "";
    char *type0;

    char *var = arg;               // La variable F
    char *in = strtok(NULL, " ");  // Le mot-clé "in"
    char *rep = strtok(NULL, " "); // Le répertoire
    printf("rep : %s\n",rep);
    char *opt = strtok(NULL," ");

    while (strcmp(opt, "{")!=0 && *opt !='\0'){    
        printf("opt : |%s|\n",opt);
        if (strcmp(opt, "-A")==0){
        
            opt_A=1;
        }   
        if (strcmp(opt, "-r")==0){
            opt_r=1;
        }
        if (strcmp(opt, "-e")==0){
            ext = strtok(NULL, " ");
        }
        printf("before0");  
        if (strcmp(opt, "-t")==0){
            printf("before");            
            type0 = strtok(NULL, " ");
            printf("type0 : ",type0);
        }
        
        opt = strtok(NULL," ");
    }

    if (var && in && rep && strcmp(in, "in") == 0) {
        char *cmd_start = strtok(NULL, "}"); // On récupère le début après la première '{'
        if (cmd_start == NULL) {
            fprintf(stderr, "Erreur: '{' manquant\n");
            *last_return = 1;
            return 1;
        }
        if (have_opt){
            cmd_start++;
        }
        //printf("cmd_start : %s\n",cmd_start);

        char full_command[MAX_CMD_LENGTH] = {0}; // Pour la commande complète
        strcat(full_command, cmd_start);
        //strcat(full_command, " ");

        char *segment = strtok(NULL, "}");       // Premier segment jusqu'à '}'

        while (segment != NULL) {
            // Ajouter le segment à la commande complète avec un espace
            strcat(full_command, segment);
            strcat(full_command, "}");

            // Vérifier s'il reste quelque chose après '}'
            segment = strtok(NULL, "}"); // Chercher le prochain segment
        }

        // Nettoyer la commande finale (supprimer les espaces inutiles)
        char *cmd_final = full_command;
        while (*cmd_final == ' ' || *cmd_final == '\t') {
            cmd_final++;
        }

        // Debug : Afficher la commande complète
        printf("DEBUG: Commande extraite pour la boucle : %s \n", cmd_final);

        if (strlen(var) == 1) {
            //strcat(full_command, "\0");
            *last_return = fsh_for(rep, cmd_final,opt_A,opt_r,ext,0);
        } else {
            fprintf(stderr, "Syntaxe incorrecte: for F in REP { CMD }\n");
            *last_return = 1;
        }
    } else {
        fprintf(stderr, "Erreur: syntaxe incorrecte, la commande doit être : for F in REP { CMD }\n");
        *last_return = 1;
    }

    return 0;
}

