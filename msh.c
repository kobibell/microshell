#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MSH_TOK_BUFSIZE 64
#define MSH_TOK_DELIM " \t\r\n\a"

// Declare function prototypes
char *msh_read_line(void);
char **msh_split_line(char *line);
int msh_launch(char **args);
int msh_execute(char **args);
int msh_cd(char **args);
int msh_help(char **args);
int msh_exit(char **args);

// Define builtin commands
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &msh_cd,
    &msh_help,
    &msh_exit
};

int msh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Implement the read line function
char *msh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    getline(&line, &bufsize, stdin);
    return line;
}

// Implement the split line function
char **msh_split_line(char *line) {
    int bufsize = MSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "msh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, MSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "msh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, MSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

// Implement the launch function
int msh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("msh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("msh");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// Implement the execute function
int msh_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < msh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return msh_launch(args);
}

// Implement the cd function
int msh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "msh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("msh");
        }
    }
    return 1;
}

// Implement the help function
int msh_help(char **args) {
    int i;
    printf("Microshell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < msh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

// Implement the exit function
int msh_exit(char **args) {
    return 0;
}

int main(int argc, char **argv) {
    char *line;
    char **args;
    int status;

    do {
        printf("cssc1432> ");
        line = msh_read_line();
        args = msh_split_line(line);
        status = msh_execute(args);

        free(line);
        free(args);
    } while (status);

    return 0;
}
