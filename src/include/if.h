#ifndef IF_H
#define IF_H

int handle_if(char **tokens, int *nb_tokens, const char *file);

int execute_block(char **tokens, int start, int end, const char *file);

int evaluate_condition(char **tokens, int start, int end);

#endif