/*

Name of file: msh.c
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
#include "helper.h"

#define MAX_ARGS 10


void execute_command(char *command) {
    
    // Check for built-in commands
    if (strncmp(command, "cd ", 3) == 0) {
        builtin_cd(command + 3);
    } else if (strcmp(command, "help") == 0) {
        builtin_help();
    } else if (strcmp(command, "exit") == 0) {
        exit(0);
    } else {
        // Check if there's a pipe in the command
        char *pipe_location = strchr(command, '|');
        if (pipe_location != NULL) {
            execute_pipes(command);
        } else {
            // Check if the command contains "grep"
            char *grep_location = strstr(command, "grep");
            if (grep_location != NULL) {
                // Split the command into arguments
                char *args[MAX_ARGS] = {NULL};
                split_command(command, args);

                // Create a new process and execute the grep command
                pid_t pid = fork();
                if (pid == 0) {
                    // Child process
                    char *grep_args[MAX_ARGS] = {NULL};
                    int i = 0;
                    int j = 0;
                    while (args[i] != NULL) {
                        if (strcmp(args[i], "grep") == 0) {
                            // Copy the "grep" argument as the first element
                            grep_args[j++] = args[i];
                        } else {
                            grep_args[j++] = args[i];
                        }
                        i++;
                    }

                    execvp("grep", grep_args); // Now, grep_args[0] contains "grep"
                    perror("execvp");
                    exit(1);
                } else if (pid > 0) {
                    // Parent process
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    perror("fork");
                    exit(1);
                }
            } else {
                // Split the command into arguments
                char *args[MAX_ARGS] = {NULL};
                split_command(command, args);

                // Create a new process and execute the program with arguments
                pid_t pid = fork();
                if (pid == 0) {
                    // Child process
                    execvp(args[0], args); // Now, args[0] contains the program name
                    perror("execvp");
                    exit(1);
                } else if (pid > 0) {
                    // Parent process
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    perror("fork");
                    exit(1);
                }
            }
        }
    }
}




int main() {
    char username[100] = "cssc1432"; // Replace with your username
    char input[1024];

    while (1) {
        printf("%s%% ", username);
        fgets(input, sizeof(input), stdin);

        // Remove the newline character
        input[strcspn(input, "\n")] = 0;

        execute_command(input);
    }

    return 0;
}
