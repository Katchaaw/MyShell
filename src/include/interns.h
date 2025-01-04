#ifndef INTERNS_H
#define INTERNS_H

/**
 * Affiche le répertoire de travail courant.
 *
 * @return 0 en cas de succès, 1 en cas d'erreur.
 */
int fsh_pwd();


/**
 * Change le répertoire de travail courant.
 *
 * Si `path` est `NULL`, retourne au répertoire `$HOME`.
 * Si `path` est "-", retourne au répertoire précédent.
 *
 * @param path Chemin vers le nouveau répertoire ou commande spéciale (`NULL` ou "-").
 * @return 0 en cas de succès, 1 en cas d'erreur.
 */
int fsh_cd(const char *path);


/**
 * Quitte le shell avec le code de retour spécifié.
 *
 * Si aucun code n'est fourni, utilise le dernier code de retour enregistré.
 *
 * @param exit_code Code de retour (par défaut, celui de la dernière commande).
 * @return Ne retourne pas (termine le programme).
 */
int fsh_exit(int exit_code);


/**
 * Affiche le type de fichier spécifié par un chemin.
 *
 * Les types possibles incluent : fichier régulier, répertoire, lien symbolique, etc.
 *
 * @param path Chemin vers le fichier dont le type doit être affiché.
 * @return 0 si le type est affiché avec succès, 1 en cas d'erreur.
 */
int fsh_ftype(const char *path);


/**
 * Gère les commandes internes du shell.
 *
 * Vérifie si une commande est interne, l'exécute si c'est le cas, ou renvoie une erreur sinon.
 *
 * @param command Nom de la commande interne.
 * @param arg Argument de la commande (peut être `NULL`).
 * @param last_return Pointeur vers le dernier code de retour (modifié si la commande réussit).
 * @return 0 si une commande interne a été traitée, 1 si la commande n'est pas interne ou erreur.
 */
int handle_interns(char *command, char *arg, int *last_return);

#endif
