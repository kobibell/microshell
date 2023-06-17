#ifndef MSH_H
#define MSH_H

char *msh_read_line(void);
char **msh_split_line(char *line);
int msh_launch(char **args);
int msh_execute_pipe(char ***commands);
int msh_execute(char **args);
int msh_cd(char **args);
int msh_help(char **args);
int msh_exit(char **args);

#endif

