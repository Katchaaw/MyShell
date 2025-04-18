#include "for.h"
#include "if.h"
#include "commands.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "externs.h"


int fsh_for(const char *rep, const char *cmd, int opt_A, int opt_r, const char *opt_ext, char opt_type, char variable, int opt_p, int max_p) { 
    int last_return = 0;  
    int active_processes = 0; // Nombre de processus actifs
    
    // Ouverture du répertoire
    DIR *dir = opendir(rep);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    // Parcours des fichiers du répertoire.
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && (last_was_signal !=2)) {
        // Ignorer les entrées "." et ".." si l'option -A est activée
        if ((opt_A && strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) {
            continue;
        }

        // Ignorer les fichiers cachés si l'option -A n'est pas activée
        if (!opt_A && entry->d_name[0] == '.') {
            continue;
        }

        // Construire le chemin complet du fichier/répertoire courant
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", rep, entry->d_name);

        // Gestion de l'option -e (filtrage par extension)
        if (opt_ext && *opt_ext) {
            char *dot = strrchr(filepath, '.'); // Trouver la dernière occurrence de "."
            if (dot && strcmp(dot + 1, opt_ext) == 0) {
                *dot = '\0'; // Supprime l'extension
            } 
            else {
                continue; 
            }
        }
        
        // Récursion si l'option -r est activée et si le fichier courant est un répertoire.
        if (opt_r) {
            struct stat file_stat;
            if (stat(filepath, &file_stat) == 0 && S_ISDIR(file_stat.st_mode)) {
                fsh_for(filepath, cmd, opt_A, opt_r, opt_ext, opt_type, variable, opt_p, max_p);
            }
        }

        // Gestion de l'option -t (filtrage par type de fichier)
        if (opt_type) {
            struct stat file_stat;
            if (stat(filepath, &file_stat) == -1) {
                perror("Erreur lors de l'obtention des informations du fichier");
                continue;
            }

            char type_char = '\0';
            if (S_ISREG(file_stat.st_mode)) type_char = 'f';       // Fichier régulier
            else if (S_ISDIR(file_stat.st_mode)) type_char = 'd';  // Répertoire
            else if (S_ISLNK(file_stat.st_mode)) type_char = 'l';  // Lien symbolique
            else if (S_ISFIFO(file_stat.st_mode)) type_char = 'p'; // FIFO

            if (type_char != opt_type) {
                continue;
            }
        }

        // Gestion de l'option -p (traitement parallèle)
        if (opt_p) {
            pid_t pid = fork();
            if (pid == 0) {
                // Processus enfant
                exit(execute_command(cmd, filepath, filepath, variable));
            } else if (pid > 0) {
                // Processus parent
                active_processes++;
            } else {
                perror("Erreur lors de la création du processus");
                closedir(dir);
                return 1;
            }

            // Mise à jour après l'attente d'un processus
            while (active_processes >= max_p) {
                int status;
                pid_t child_pid = wait(&status);
                if (child_pid > 0) {
                    active_processes--;

                    if (WIFEXITED(status)) {
                        int child_return = WEXITSTATUS(status);
                        if (child_return > last_return) {
                            last_return = child_return;
                        }
                    }
                }
            }
        } else {
            // Exécution séquentielle
            int current_return = execute_command(cmd, filepath, filepath, variable);
            if (current_return > last_return) {
                last_return = current_return;
            }
        }

    }

    // Attendre la fin de tous les processus enfants
    while (active_processes > 0) {
        wait(NULL);
        active_processes--;
    }

    closedir(dir);
    return last_return;
}



int handle_for(char *arg, int *last_return) {
    int have_opt = 0;
    int opt_A = 0;    // Option pour inclure les fichiers cachés
    int opt_r = 0;    // Option pour activer la récursion
    char *ext = "";   // Extension à filtrer
    char *type0 = ""; // Type de fichier à filtrer
    int opt_p = 0;
    int max_p =0;
    char *var = arg;
    char *in = strtok(NULL, " ");
    char *rep = strtok(NULL, " "); // Répertoire à parcourir
    char *opt = strtok(NULL, " "); // Options
    // Analyse des options.
    while (strcmp(opt, "{")!=0 && *opt !='\0'){    
        if (strcmp(opt, "-A")==0){
            opt_A=1;
        }   
        if (strcmp(opt, "-r")==0){
            opt_r=1;
        }
        if (strcmp(opt, "-e")==0){
            ext = strtok(NULL, " ");
        }
        if (strcmp(opt, "-t")==0){     
            type0 = strtok(NULL, " ");
        }
        if (strcmp(opt, "-p")==0){     
            max_p = atoi(strtok(NULL, " "));
            opt_p = 1;
        }
        opt = strtok(NULL," ");
    }
    if (var && in && rep && strcmp(in, "in") == 0) {
        char *cmd_start = strtok(NULL, "}"); // On récupère le début après la première '{'
        char *verifAc = cmd_start;
        int multiAc = 0;
        if (strstr(verifAc,"{") && strstr(verifAc,"if")){
            multiAc = 1;
        }
        
        if (cmd_start == NULL) {
            perror("Erreur: '{' manquant\n");
            *last_return = 1;
            return 1;
        }

        if (have_opt){
            cmd_start++;
        }

        //Construire la commande complète.
        char full_command[MAX_CMD_LENGTH] = {0};
        strcat(full_command, cmd_start);

        char *segment = strtok(NULL, "}"); // Premier segment jusqu'à '}'
        if (multiAc){
            strcat(full_command, "}");
        }
        while (segment != NULL) {
           strcat(full_command, segment);
           segment = strtok(NULL, "}");
           if (segment !=NULL){
                strcat(full_command, "}");
           }
        }
        
        char *cmd_final = full_command;
        // Nettoyer la commande finale (supprimer les espaces inutiles)
        while (*cmd_final == ' ' || *cmd_final == '\t') {
            cmd_final++;
        }

        // Vérification de la longueur de la variable
        if (strlen(var) == 1) {
            *last_return = fsh_for(rep, cmd_final,opt_A,opt_r,ext,*type0,*arg,opt_p,max_p);
        } 
        else {
            perror("Syntaxe incorrecte: for F in REP { CMD }\n");
            *last_return = 1;
        }
        
    } else {
        perror("Erreur: syntaxe incorrecte, la commande doit être : for F in REP { CMD }\n");
        *last_return = 1;
    }

    return 0;
}

