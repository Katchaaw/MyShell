#include "main.h"

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
    char truncated_cwd[PROMPT_TOTAL_LENGTH - FIXED_PROMPT_LENGTH + 1];

    // Obtient le répertoire courant et le tronque pour respecter la limite
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        truncate_path(cwd, truncated_cwd, PROMPT_TOTAL_LENGTH - FIXED_PROMPT_LENGTH);
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
