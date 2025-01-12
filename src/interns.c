#include "interns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>


int fsh_pwd() {
    // Buffer pour stocker le chemin du répertoire courant
    char cwd[PATH_MAX]; 

    // Récupère et affiche le répertoire courant
    if (getcwd(cwd, sizeof(cwd)) != NULL) {  
        write(1, cwd, strlen(cwd));
        write(1, "\n", 1);
        return 0;
    } 

    perror("pwd");
    return 1;
}


int fsh_cd(const char *path) {
    static char previous_dir[PATH_MAX]; // Stocke le répertoire précédent
    char current_dir[PATH_MAX];         // Buffer pour le répertoire courant
    const char *home = getenv("HOME");  // Récupère $HOME

    // Récupère le répertoire courant
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("cd");
        return 1;
    }

    // Si path est NULL, on retourne vers $HOME
    if (path == NULL) {
        if (home == NULL) {
            perror("Erreur: HOME non défini\n");
            return 1;
        }
        // Change au répertoire HOME
        if (chdir(home) == 0) { 
            strcpy(previous_dir, current_dir); // Mémorise le répertoire courant
            return 0;
        }
        
        perror("cd");
        return 1;
    }

    // Si path est "-", on retourne au précédent
    if (strcmp(path, "-") == 0) {
        if (chdir(previous_dir) == 0) { // Change au répertoire précédent
            strcpy(previous_dir, current_dir); // Mémorise le répertoire courant
            return 0;
        } 
        perror("cd");
        return 1;
    }

    // Sinon, on change pour le répertoire spécifié
    if (chdir(path) == 0) { 
        strcpy(previous_dir, current_dir); // Mémorise le répertoire courant
        return 0;
    }

    perror("cd");
    return 1;
}


int fsh_exit(int exit_code) {
    exit(exit_code);
}


int fsh_ftype(const char *path) {
    struct stat path_stat; // Structure pour les métadonnées du fichier.

    // Récupère les métadonnées
    if (lstat(path, &path_stat) == -1) { 
        perror("ftype");
        return 1;
    }

    // Vérifie et affiche le type de fichier
    switch (path_stat.st_mode & S_IFMT) {
        case S_IFREG:
            write(1,"regular file", 12) ;
            write(1, "\n", 1);      
            break;
        case S_IFDIR:
            write(1, "directory", 9) ;
            write(1, "\n", 1);
            break;
        case S_IFLNK:
            write(1, "symbolic link", 13) ;
            write(1, "\n", 1);
            break;
        case S_IFIFO:
            write(1, "named pipe", 10) ;
            write(1, "\n", 1);
            break;
        default:
            write(1, "other", 5);
            write(1, "\n", 1);
            break;
    }

    return 0;
}


int handle_interns(char *command, char *arg, int *last_return) {

    // Vérification supplémentaire pour les arguments multiples
    if (strcmp(command, "cd") == 0) {
        if (arg && strtok(NULL, " ") != NULL) { // Plus d'un argument détecté
            perror("cd: too many arguments\n");
            *last_return = 1;
            return 0;
        }
        *last_return = fsh_cd(arg);
    }

    else if (strcmp(command, "pwd") == 0) {
        if (arg) { // pwd ne prend pas d'arguments
            const char *msg_prefix = "pwd: ";
            const char *msg_suffix = ": invalid argument\n";
            write(STDERR_FILENO, msg_prefix, strlen(msg_prefix));    // "pwd: "
            write(STDERR_FILENO, arg, strlen(arg));                  // L'argument
            write(STDERR_FILENO, msg_suffix, strlen(msg_suffix));    // ": invalid argument\n"
            *last_return = 1;
            return 0;
        }
        *last_return = fsh_pwd();
    }

    else if (strcmp(command, "exit") == 0) {
        if (arg && strtok(NULL, " ") != NULL) { // Plus d'un argument détecté
            perror("exit: too many arguments\n");
            *last_return = 1;
            return 0;
        }
        int exit_code = arg ? atoi(arg) : *last_return;
        fsh_exit(exit_code);
        return -1; 
    }

    else if (strcmp(command, "ftype") == 0) {
        if (!arg || strtok(NULL, " ") != NULL) { // Pas d'argument ou trop d'arguments
            perror("ftype: invalid number of arguments\n");
            *last_return = 1;
            return 0;
        }
        *last_return = fsh_ftype(arg);
    }
    
    else {
        return 1; // Commande non interne
    }

    return 0;
}
