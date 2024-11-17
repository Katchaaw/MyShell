#ifndef COMMANDS_H
#define COMMANDS_H

// Définition de la longueur maximale du chemin.
#define PATH_MAX 4096

// Affiche le répertoire de travail courant.
// Retourne 0 en cas de succès, 1 sinon.
int fsh_pwd();


// Change le répertoire de travail courant.
// Paramètre -> chemin vers le nouveau répertoire.
// Retourne 0 en cas de succès, 1 sinon.
int fsh_cd(const char *path);


// Arrête le Programme.
// Si exit_code est omis, utilise la valeur de retour de la dernière commande exécutée.
int fsh_exit(int exit_code);


// Affiche le type de fichier spécifié par le chemin donné.
// Retourne 0 si le type est affiché avec succès, 1 sinon.
int fsh_ftype(const char *path);

// Parcourt les fichiers d'un répertoire donné et exécute une commande pour chaque fichier.
// Paramètres:
//  - rep : chemin vers le répertoire à parcourir.
//  - cmd_start : commande à exécuter pour chaque fichier, avec $F représentant le chemin du fichier.
// Retourne 0 en cas de succès, 1 sinon.
int fsh_for(const char *rep, const char *cmd_start);

#endif
