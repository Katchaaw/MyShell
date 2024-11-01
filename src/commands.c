#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "commands.h"

// Affiche le répertoire courant
int fsh_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror("pwd");
        return 1;
    }
}

// Change de répertoire
int fsh_cd(const char *path) {
    static char previous_dir[PATH_MAX];
    char current_dir[PATH_MAX];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("cd");
        return 1;
    }

    // Si path est "-", on retourne au précédent
    if (path == NULL || strcmp(path, "-") == 0) {
        if (chdir(previous_dir) == 0) {
            strcpy(previous_dir, current_dir);
            fsh_pwd();
            return 0;
        } else {
            perror("cd");
            return 1;
        }
    }

    // Sinon on change pour le path donné
    if (chdir(path) == 0) {
        strcpy(previous_dir, current_dir);
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
    struct stat path_stat;

    if (stat(path, &path_stat) == -1) {
        perror("ftype");
        return 1;
    }

    if (S_ISREG(path_stat.st_mode)) {
        printf("regular file\n");
    } else if (S_ISDIR(path_stat.st_mode)) {
        printf("directory\n");
    } else if (S_ISLNK(path_stat.st_mode)) {
        printf("symbolic link\n");
    } else if (S_ISFIFO(path_stat.st_mode)) {
        printf("named pipe\n");
    } else {
        printf("other\n");
    }
    return 0;
}
