#ifndef TOKENIZER_H
#define TOKENIZER_H

#define MAX_TOKENS 100

void cleanup_tokens(char *tokens[], int *nb_tokens);
int tokenizer(char *line, char *tokens[], int *nb_tokens, const char *delimiter);

#endif