#ifndef COMMANDS_H
#define COMMANDS_H

#define PATH_MAX 4096  // Valeur typique pour les chemins sous Linux

int fsh_pwd();
int fsh_cd(const char *path);
int fsh_exit(int exit_code);
int fsh_ftype(const char *path);

#endif
