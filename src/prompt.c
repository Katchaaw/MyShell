#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include "externs.h"
#include "interns.h"


int get_fixed_prompt_length(int last_return) {
    int len = 3;  // Longueur de base pour les crochets et "$ ".
    
    if (last_return == 0) {
        len += 1;  // "0" occupe un unique caractère.
    } else {
       len += (int)log10(last_return) + 1;  // Longueur du nombre en caractères.
    }

    return len; 
}


void truncate_path(const char *path, char *truncated_path, size_t max_length) {
    size_t len = strlen(path); //Longueur de base pour les crochets et "$ ".
    
    // Copie sans modification si le chemin est court.
    if (len <= max_length) {
        strcpy(truncated_path, path);
    } 

    //Troncature. "+3" pour tenir compte de la longueur des "..." ajoutés.
    else {
        snprintf(truncated_path, max_length + 1, "...%s", path + len - max_length + 3);
    }
}


char* generate_prompt(int last_return) {
    static char prompt[PROMPT_BUFFER_SIZE]; // Buffer statiue pour le prompt
    char cwd[PATH_MAX];                     // Buffer pour le répertoire courant.
    char truncated_cwd[PROMPT_TOTAL_LENGTH + 1];  // Buffer pour le chemin tronqué.

    // Calculer dynamiquement la longueur pour la partie "[last_return]$ ".
    int fixed_prompt_len = get_fixed_prompt_length(last_return);

    // Récupère le répertoire courant
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Tronque le chemin pour respecter la taille maximale
        truncate_path(cwd, truncated_cwd, PROMPT_TOTAL_LENGTH - fixed_prompt_len);  // Utiliser la longueur dynamique
    } else {
        // Erreur de récupération du répertoire courant
        perror("Erreur getcwd");
        strcpy(truncated_cwd, "?");
    }

    if (last_was_signal) {
         snprintf(prompt, sizeof(prompt), "\001\033[91m\002[SIG] \001\033[34m\002%s$ \001\033[36m\002", truncated_cwd);
    } 
    else {
        snprintf(prompt, sizeof(prompt), "\001\033[%sm\002[%d]\001\033[00m\002\001\033[34m\002%s$ \001\033[36m\002",
                last_return == 0 ? "32" : "91",
                last_return,
                truncated_cwd);
    }

    return prompt;
}
