#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void tokenize(char *input, int *token_count, char ***token_vector) {
    int size = 10; 
    *token_vector = malloc(size * sizeof(char *));
    *token_count = 0;

    char *token;
    while ((token = strsep(&input, " \t")) != NULL) {
        if (*token != '\0') {
            (*token_vector)[*token_count] = token;
            (*token_count)++;

            if (*token_count >= size) {
                size *= 2;
                *token_vector = realloc(*token_vector, size * sizeof(char *));
            }
        }
    }
    (*token_vector)[*token_count] = NULL; 
}
int main() {
    char input[1024];
    char **token_vector;
    int token_count;

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break; 
        }

        if (input[0] == '\n') {
            continue; 
        }
        tokenize(input, &token_count, &token_vector);

        if (strcmp(token_vector[0], "exit") == 0) {
            free(token_vector);
            break;
        }
        int pid = fork();
        if (pid == 0) { 
            if (execvp(token_vector[0], token_vector) == -1) {
                perror("Error executing the program");
            }
            exit(EXIT_FAILURE);
        } else if (pid > 0) { 
            wait(NULL); 
        } else {
            perror("Fork failed");
        }

        free(token_vector);
    }
    return 0;
}
