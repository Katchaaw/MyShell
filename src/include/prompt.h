#ifndef PROMPT_H
#define PROMPT_H

#define PROMPT_TOTAL_LENGTH 29 // Taille max affichable sur le prompt
#define PROMPT_BUFFER_SIZE 64  // Taille du buffer pour `prompt`, plus large pour inclure les codes de couleur

void truncate_path(const char *path, char *truncated_path, size_t max_length);
char* generate_prompt(int last_return);


#endif