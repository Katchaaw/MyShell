#include "main.h"
#include <math.h>  // pour log10()

// Fonction pour calculer la longueur de la partie avec le code retour
int get_fixed_prompt_length(int last_return) {
    int len = 3;  // pour les crochets et $
    if (last_return == 0) {
        len += 1;  // "0" a une seule longueur
    } else {
        return (int) log10(last_return) + 4;  // pour la longueur du nombre
    }
    return len; 
}

void truncate_path(const char *path, char *truncated_path, size_t max_length) {
    size_t len = strlen(path);
    if (len <= max_length) {
        strcpy(truncated_path, path);
    } else {
        snprintf(truncated_path, max_length + 1, "...%s", path + len - max_length + 3);
    }
}

char* generate_prompt(int last_return) {
    static char prompt[PROMPT_BUFFER_SIZE];  // Utilisation de `PROMPT_BUFFER_SIZE` pour gérer les séquences d’échappement
    char cwd[PATH_MAX];
    char truncated_cwd[PROMPT_TOTAL_LENGTH + 1];  // Taille du buffer ajustée pour inclure tout le prompt

    // Calculer la longueur dynamique de la partie "[last_return]$ "
    int fixed_prompt_len = get_fixed_prompt_length(last_return);

    // Obtient le répertoire courant et le tronque pour respecter la limite
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        truncate_path(cwd, truncated_cwd, PROMPT_TOTAL_LENGTH - fixed_prompt_len);  // Utiliser la longueur dynamique
    } else {
        perror("Erreur getcwd");
        strcpy(truncated_cwd, "?");
    }

    // Prépare le prompt avec les couleurs et en respectant la limite totale de 30 caractères
    snprintf(prompt, sizeof(prompt),
             "\001\033[%sm\002[%d]\001\033[00m\002\001\033[34m\002%s$ \001\033[36m\002",
             last_return == 0 ? "32" : "91", last_return, truncated_cwd);

    return prompt;
}
