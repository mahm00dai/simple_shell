#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

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
void free_tokens(char **tokens);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
void setenv_cmd(char **argv);
void unsetenv_cmd(char **argv);
char *replace_env_var(const char *str);
void echo_cmd(char **argv);
void cd_cmd(char **argv);
void pwd(void);
void ls_cmd(char **argv);
void sigint_handler(int sig);
void print_total_blocks(unsigned long total_blocks);
void print_file_name(const char *name);
void process_entries(DIR *dir, const char *path,
		int long_format, int show_hidden);
int _read_from_buffer(char **lineptr, size_t *n, char *buffer,
			     size_t *pos, ssize_t bytes_read, size_t *total_read);
int _realloc_lineptr(char **lineptr, size_t *n);
char *handle_env_var(char *result, char **ptr, const char **str);
char *extract_env_var(const char **str, size_t *var_len);
char *append_env_value(char *result, char *ptr, const char *env_value);
char *get_env_var_value(const char *env_var);

#endif /* MAIN_H */

