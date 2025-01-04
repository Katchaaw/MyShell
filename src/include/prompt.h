#ifndef PROMPT_H
#define PROMPT_H

// Taille maximale du chemin affiché dans le prompt.
#define PROMPT_TOTAL_LENGTH 29 

// Taille du buffer pour le prompt, plus large pour inclure les codes couleur.
#define PROMPT_BUFFER_SIZE 64

#include <stddef.h>

/**
 * Tronque un chemin pour qu'il ne dépasse pas une longueur maximale spécifiée.
 *
 * Si le chemin est plus long que `max_length`, il est abrégé en préfixant par "..."
 *
 * @param path Chemin complet à tronquer.
 * @param truncated_path Buffer où stocker le chemin tronqué.
 * @param max_length Longueur maximale autorisée pour le chemin tronqué.
 */
void truncate_path(const char *path, char *truncated_path, size_t max_length);


/**
 * Génère la chaîne de caractères représentant le prompt du shell.
 *
 * Le prompt inclut :
 * - Le dernier code de retour entre crochets, coloré en vert si succès, rouge sinon.
 * - Le répertoire courant, potentiellement tronqué pour respecter la taille maximale.
 *
 * @param last_return Dernier code de retour d'une commande.
 * @return Une chaîne statique contenant le prompt formaté.
 */
char* generate_prompt(int last_return);

#endif