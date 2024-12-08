#include "main.h"

int execute_external_command(char *cmd, char **args) {
    // Création d'un fils
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    // Dans le processus fils.
    if (pid == 0) {
        // Exécute la commande.
        if (execvp(cmd, args) == -1) {
            perror("execvp"); 
            exit(1);
        }
    } 

    // Dans le processus père
    else {
        int status;
        waitpid(pid, &status, 0); // Attend que le processus fils se termine

        // Retourne le code de sortie du fils
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } 

        return 1; // Erreur d'exécution
        
    }

    return 0;
}
