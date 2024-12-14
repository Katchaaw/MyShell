#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

int handle_redirections(char **tokens, int *nb_tokens);

void save_redirections();

void reset_redirections();

void close_saved_redirections();

#endif
