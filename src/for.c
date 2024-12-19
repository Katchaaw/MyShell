#include "main.h"

int fsh_for(const char *rep, const char *cmd,int opt_A, int opt_r,const char *opt_ext,char opt_type) { //char opt_type
    //printf("DEBUG: Début de la boucle 'for' avec répertoire : |%s| et commande : |%s|\n", rep, cmd);
    //printf("\noption -A : [%d]\noption -r : [%d]\noption -e : [%s]\n, option -t : [%d]\n",opt_A,opt_r,opt_ext,opt_type);
    //printf("après print");
    // Ouverture du répertoire
    //char *directory;  
    int last_return=0;  
    DIR *dir = opendir(rep);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }


    // Parcours des fichiers du répertoire.
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((opt_A && strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) {
            continue;
        }

        // Gestion de l'option -A
        if (!opt_A && entry->d_name[0] == '.') {
            continue;
        }

        // Construction du chemin complet
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", rep, entry->d_name);

        // Gestion de l'option -e (extension)
        if (opt_ext && *opt_ext) {
            //printf("sazdazd");
            char *dot = strrchr(filepath, '.');
            if (dot && strcmp(dot + 1, opt_ext) == 0) {
                
                *dot = '\0'; // Supprime l'extension
                //printf("sazdazd:%s\n",filepath);
            } else {
                continue; // Ignorer si l'extension ne correspond pas
            }
        }
        
        // Récursion si -r et si c'est un répertoire
        if (opt_r) {
            struct stat file_stat;
            if (stat(filepath, &file_stat) == 0 && S_ISDIR(file_stat.st_mode)) {
                fsh_for(filepath, cmd, opt_A, opt_r, opt_ext, opt_type);
            }
        }


        // Gestion de l'option -t (type)
        if (opt_type) {
            struct stat file_stat;
            if (stat(filepath, &file_stat) == -1) {
                perror("Erreur lors de l'obtention des informations du fichier");
                continue;
            }

            char type_char = '\0';
            if (S_ISREG(file_stat.st_mode)) type_char = 'f';
            else if (S_ISDIR(file_stat.st_mode)) type_char = 'd';
            else if (S_ISLNK(file_stat.st_mode)) type_char = 'l';
            else if (S_ISFIFO(file_stat.st_mode)) type_char = 'p';

            if (type_char != opt_type ){//&& !(opt_r && type_char == 'd')) {
                continue;
            }
        }

        // Exécuter la commande pour le fichier/répertoire courant
        int last_returnTemp = execute_command(cmd, filepath, filepath);
        if (last_returnTemp> last_return){last_return = last_returnTemp;}

        
    }
    closedir(dir);

    return last_return;
}
#define MAX_CMD_LENGTH 1024 // Taille maximale de la commande

int handle_for(char *arg, int *last_return) {
    //printf("debut for ------------------------\n");
    int have_opt = 0;
    int more_cmd = 0;
    char *secCommande;
    int opt_A =0;
    int opt_r =0;
    char *ext = "";
    char *type0= "";
    //printf("arg : |%s|",arg);
    char *var = arg;               // La variable F
    char *in = strtok(NULL, " ");  // Le mot-clé "in"
    char *rep = strtok(NULL, " "); // Le répertoire
    //printf("rep : %s\n",rep);
    char *opt = strtok(NULL," ");
    //printf("Avant,opt : |%s|\n",opt);
    while (strcmp(opt, "{")!=0 && *opt !='\0'){    
        //printf("opt : |%s|\n",opt);
        if (strcmp(opt, "-A")==0){
            opt_A=1;
        }   
        if (strcmp(opt, "-r")==0){
            opt_r=1;
        }
        if (strcmp(opt, "-e")==0){
            ext = strtok(NULL, " ");
        }
        //printf("before0");  
        if (strcmp(opt, "-t")==0){
            //printf("before");            
            type0 = strtok(NULL, " ");
            //printf("type0 : %s",type0);
        }
        
        opt = strtok(NULL," ");
    }
    //printf("fin while\n");

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
        char full_command[MAX_CMD_LENGTH] = {0};

        
    
        //char *pos = cmd_start;
        //printf("-dz-d-zd--ze-d-ze-%s\n",pos);
        /*if ((pos = strstr(pos, ";")) != NULL){
            //printf("-dz-d-aaaaazd--ze-d-ze-\n");
            more_cmd = 1;
            secCommande = strtok(cmd_start,";");
            cmd_start = strtok(NULL, "}");
        }*/
        

        // Pour la commande complète
        strcat(full_command, cmd_start);
        //strcat(full_command, " ");
        

        //printf("deuxième : %s\n",secCommande);
        //printf("cmd_start : %s\n",cmd_start);
        
        
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
       //printf("DEBUG: Commande extraite pour la boucle : %s \n", cmd_final);

        if (strlen(var) == 1) {
            //strcat(full_command, "\0");
            //printf("fin handle\n");
            //if (more_cmd){
              //  *last_return = fsh_for(rep, secCommande,opt_A,opt_r,ext,*type0);            
            //}

            *last_return = fsh_for(rep, cmd_final,opt_A,opt_r,ext,*type0);
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

