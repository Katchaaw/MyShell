#include "main.h"

int last_was_signal = 0;

int execute_external_command(char *cmd, char **args) {

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    
    // Exécution de la commande dans un processus fils
    if (pid == 0) {
        restore_default_signals();

        if (execvp(cmd, args) == -1) {
            perror("execvp"); 
            exit(1);
        }
    } 

    // Dans le processus père
    else {
        int status;
        // Attend la fin du processus fils et récupère sa valeur de retour
        waitpid(pid, &status, 0); 

        // Retourne la valeur de sortie du processus fils
        if (WIFSIGNALED(status)) {
            last_was_signal = 1;
            return 255; // Valeur de retour définie dans l'énoncé
        } 
        else if (WIFEXITED(status)) {
            last_was_signal = 0;
            return WEXITSTATUS(status);
        }

        last_was_signal = 1;
        return 1; 
    }

    return 0;
}
