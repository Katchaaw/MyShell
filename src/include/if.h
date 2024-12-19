#ifndef IF_H
#define IF_H

int handle_if_else(char **tokens, int *nb_tokens, int *last_return);

int execute_block(char **tokens, int start, int end, const char *file);

int evaluate_condition(char **tokens, int start, int end);

#endif
