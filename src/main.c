#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(int argc, char *argv[]) {
    char *line;

    // Boucle principale de l'interpréteur
    while (1) {
        rl_outstream = stderr;
        line = readline("mon joli prompt $ ");
        if (!line) {
            // Gestion de la fin d'entrée (Ctrl+D)
            printf("\n");
            break;
        }
        add_history(line);
        execute_line(line);
        free(line);
    }

    return 0;
}
