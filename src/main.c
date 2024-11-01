#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "commands.h"

#define PROMPT_SIZE 32

int main() {
    char *line;
    int last_return = 0;

    rl_outstream = stderr;

    while (1) {
        char prompt[PROMPT_SIZE];
        snprintf(prompt, PROMPT_SIZE, "\001\033[%sm\002[%d]\001\033[00m\002$ ", last_return == 0 ? "32" : "91", last_return);

        line = readline(prompt);

        if (!line) {
            printf("exit\n");
            break;
        }

        add_history(line);

        char *command = strtok(line, " ");
        char *arg = strtok(NULL, " ");

        if (command == NULL) {
            free(line);
            continue;
        }

        if (strcmp(command, "pwd") == 0) {
            last_return = fsh_pwd();
        } else if (strcmp(command, "cd") == 0) {
            last_return = fsh_cd(arg);
        } else if (strcmp(command, "exit") == 0) {
            int exit_code = arg ? atoi(arg) : last_return;
            free(line);
            fsh_exit(exit_code);
        } else if (strcmp(command, "ftype") == 0) {
            if (arg) {
                last_return = fsh_ftype(arg);
            } else {
                fprintf(stderr, "ftype: argument requis\n");
                last_return = 1;
            }
        } else {
            pid_t pid = fork();
            if (pid == -1) {
                perror("Erreur fork");
                last_return = 1;
            } else if (pid == 0) {
                if (execlp(command, command, arg, (char *)NULL) == -1) {
                    perror("Erreur exec");
                    exit(1);
                }
            } else {
                int status;
                waitpid(pid, &status, 0);
                last_return = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
            }
        }

        free(line);
    }

    return 0;
}
