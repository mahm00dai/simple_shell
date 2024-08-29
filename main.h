#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* External declaration for the environ variable */
extern char **environ;

/* Function prototypes */
void simple_shell(void);
char *read_command(void);
int process_command(char *command);
int tokenize_command(char *command, char *argv[]);
void execute_command(char **argv);
void print_env(void);
void handle_error(void);
void exit_shell(int status);
size_t count_tokens(const char *str, char delim);
char *copy_token(const char *start, size_t length);
char **tokenize(char *str, char delim);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
void setenv_cmd(char **argv);
void unsetenv_cmd(char **argv);
char *replace_env_var(const char *str);
void echo_cmd(char **argv);
void cd_cmd(char **argv);
void pwd(void);
void ls_cmd(char **argv);

#endif /* MAIN_H */

