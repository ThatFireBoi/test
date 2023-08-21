#include "simple_shell.h"

/**
 * execute - Function to execute a command using fork and execve
 *
 * @command: The command to execute
 *
 * @env: The environment variables
 */

void execute(char *command, char **env) {
    pid_t child_pid;
    char *token = NULL;
    char **tokens = NULL;
    int token_count = 0;

    /* Tokenize the command string */
    token = strtok(command, " \n");
    if (token == NULL)
        return;

    /* Tokenize the command and store tokens in an array */
    while (token != NULL) {
        tokens = realloc(tokens, sizeof(char *) * (token_count + 2));
        if (tokens == NULL) {
            perror("realloc");
            return;
        }
        tokens[token_count] = token;
        token_count++;
        token = strtok(NULL, " \n");
    }
    tokens[token_count] = NULL; /* Set the last element to NULL */

    /* Create the child process */
    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        free(tokens); /* Free allocated memory */
        return;
    }

    if (child_pid == 0) { /* Child process */
        /* Execute the command using execve */
        if (execve(tokens[0], tokens, env) == -1) {
            perror("execve error");
            exit(EXIT_FAILURE); /* Exit child process with failure status */
        }
    } else { /* Parent process */
        /* Wait for the child process to complete */
        waitpid(child_pid, NULL, 0);
        free(tokens); /* Free allocated memory */
    }
}
