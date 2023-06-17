/*

Name of file: helper.c
Team: Kobi Bell and Mark Duraid
Class: CS 480
Assignment #2

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define MAX_ARGS 10

void builtin_cd(char *path) {
    if (chdir(path) != 0) {
        perror("chdir");
    }
}

void builtin_help() {
    printf("Microshell (MSH) - Available commands:\n");
    printf("cd [path] - Change the current directory to [path]\n");
    printf("help - Display this help message\n");
    printf("exit - Exit the shell\n");
}

void split_command(char *command, char **args) {
    int i = 0;
    args[i] = strtok(command, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

void execute_pipes(char *command) {
    // Split the command into parts at the pipe symbol
    char *parts[MAX_ARGS] = {NULL};
    int num_parts = 0;
    parts[num_parts] = strtok(command, "|");
    while (parts[num_parts] != NULL && num_parts < MAX_ARGS - 1) {
        num_parts++;
        parts[num_parts] = strtok(NULL, "|");
    }

    // Create a pipe for each pair of commands
    int pipefd[MAX_ARGS - 1][2];
    for (int i = 0; i < num_parts - 1; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Create a new process for each part of the command
    pid_t pids[MAX_ARGS];
    for (int i = 0; i < num_parts; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Child process
            if (i < num_parts - 1) {
                close(pipefd[i][0]); // Close the read end of the current pipe
                dup2(pipefd[i][1], STDOUT_FILENO); // Redirect stdout to the write end of the current pipe
                close(pipefd[i][1]);
            }
            if (i > 0) {
                close(pipefd[i - 1][1]); // Close the write end of the previous pipe
                dup2(pipefd[i - 1][0], STDIN_FILENO); // Redirect stdin to the read end of the previous pipe
                close(pipefd[i - 1][0]);
            }

            // Split the command into arguments and execute it
            char *args[MAX_ARGS] = {NULL};
            split_command(parts[i], args);
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(1);
            }
        }
    }

    // Close all pipes in the parent process
    for (int i = 0; i < num_parts - 1; i++) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    // Wait for all child processes to finish
    int status;
    for (int i = 0; i < num_parts; i++) {
        waitpid(pids[i], &status, 0);
    }
}
