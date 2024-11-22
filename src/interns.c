#include "main.h"

// Affiche le répertoire courant
int fsh_pwd() {
    char cwd[PATH_MAX]; // Buffer pour stocker le chemin du répertoire courant
    if (getcwd(cwd, sizeof(cwd)) != NULL) { // Récupère le répertoire courant
        printf("%s\n", cwd); // Affiche le chemin
        return 0;
    } else {
        perror("pwd");
        return 1;
    }
}

// Change de répertoire
int fsh_cd(const char *path) {
    static char previous_dir[PATH_MAX]; // Stocke le précédent répertoire
    char current_dir[PATH_MAX]; // Buffer pour le répertoire courant
    const char *home = getenv("HOME"); // Récupère $HOME

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) { // Récupère le répertoire courant
        perror("cd");
        return 1;
    }

    // Si path est NULL, on retourne vers $HOME
    if (path == NULL) {
        if (home == NULL) {
            fprintf(stderr, "Erreur: HOME non défini\n");
            return 1;
        }
        if (chdir(home) == 0) { // Change au répertoire HOME
            strcpy(previous_dir, current_dir); // Mémorise le répertoire courant
            return 0;
        }
        else {
            perror("cd");
            return 1;
        }
    }

    // Si path est "-", on retourne au précédent
    if (strcmp(path, "-") == 0) {
        if (chdir(previous_dir) == 0) { // Change au répertoire précédent
            strcpy(previous_dir, current_dir); // Mémorise le répertoire courant
            return 0;
        } else {
            perror("cd");
            return 1;
        }
    }

    // Sinon, on change pour le path donné
    if (chdir(path) == 0) { // Change au chemin spécifié
        strcpy(previous_dir, current_dir); // Mémorise le répertoire courant
        return 0;
    } else {
        perror("cd");
        return 1;
    }
}

// Quitte le shell avec le code de retour donné
int fsh_exit(int exit_code) {
    exit(exit_code);
}

// Affiche le type de fichier
int fsh_ftype(const char *path) {
    struct stat path_stat; // Structure pour stocker les informations sur le fichier

    if (lstat(path, &path_stat) == -1) { // Récupère les informations sur le fichier
        perror("ftype");
        return 1;
    }

    // Vérifie le type de fichier et affiche le résultat
    switch (path_stat.st_mode & S_IFMT) {
        case S_IFREG:
            printf("regular file\n");
            break;
        case S_IFDIR:
            printf("directory\n");
            break;
        case S_IFLNK:
            printf("symbolic link\n");
            break;
        case S_IFIFO:
            printf("named pipe\n");
            break;
        default:
            printf("other\n");
            break;
    }

    return 0;
}

int handle_interns(char *command, char *arg, int *last_return) {
    if (strcmp(command, "pwd") == 0) {
        *last_return = fsh_pwd();
    }
    else if (strcmp(command, "cd") == 0) {
        *last_return = fsh_cd(arg);
    }
    else if (strcmp(command, "exit") == 0) {
        int exit_code = arg ? atoi(arg) : *last_return;
        fsh_exit(exit_code);
        return -1; // On ne retourne jamais après un exit, mais il est nécessaire de renvoyer un statut
    }
    else if (strcmp(command, "ftype") == 0) {
        if (arg) {
            *last_return = fsh_ftype(arg);
        } else {
            fprintf(stderr, "ftype: argument requis\n");
            *last_return = 1;
        }
    }
    
    // Commande non interne
    else {
        return 1;
    }
    
    return 0; // Commande interne traitée avec succès
}
