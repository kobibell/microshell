/*

Name of file: helper.h
Team: Kobi Bell and Mark Duraid
Class: CS 480
Assignment #2

*/

#ifndef HELPERS_H
#define HELPERS_H

void builtin_cd(char *path);
void builtin_help();
void split_command(char *command, char **args);
void execute_pipes(char *command);

#endif
