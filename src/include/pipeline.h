#ifndef PIPELINE_H
#define PIPELINE_H

/**
 * Exécute plusieurs commandes en pipeline.
 *
 * Sépare les commandes par `|`, crée des pipes pour connecter les processus, 
 * applique les redirections et exécute chaque commande dans un processus fils.
 *
 * @param tokens Tableau des commandes à exécuter, séparées par `|`.
 *
 * @return 0 en cas de succès, 1 en cas d'erreur.
 */
int handle_pipe(char **tokens);

/**
 * Vérifie la validité de la syntaxe des pipes dans les tokens.
 *
 * S'assure qu'il n'y a pas de pipes en début/fin de chaîne ni de pipes successifs.
 *
 * @param tokens Tableau des tokens de la commande.
 *
 * @return 1 si un pipe valide est trouvé, 0 sinon.
 */
int check_pipe(char **tokens);

#endif
