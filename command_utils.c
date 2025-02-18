#include "main.h"
#include <errno.h>

/**
 * process_command - Processes and executes the command
 * @command: The command string
 *
 * Return: 0 on success, -1 on failure
 */
int process_command(char *command)
{
	char *argv[100];
	int i, argc, status = 0;

	argc = tokenize_command(command, argv);
	if (argc == 0)
	{
		free(command);
		return (0);
	}
	if (strcmp(argv[0], "exit") == 0)
	{
		if (argc > 1)
			status = atoi(argv[1]);
		for (i = 0; i < argc; i++)
			free(argv[i]);
		free(command);
		exit_shell(status);
	}
	else if (strcmp(argv[0], "env") == 0)
		print_env();
	else if (strcmp(argv[0], "setenv") == 0)
		setenv_cmd(argv);
	else if (strcmp(argv[0], "unsetenv") == 0)
		unsetenv_cmd(argv);
	else if (strcmp(argv[0], "echo") == 0)
		echo_cmd(argv);
	else if (strcmp(argv[0], "cd") == 0)
		cd_cmd(argv);
	else if (strcmp(argv[0], "pwd") == 0)
		pwd();
	else if (strcmp(argv[0], "ls") == 0)
		ls_cmd(argv);
	else
		execute_command(argv);

	/* Free memory for all arguments */
	for (i = 0; i < argc; i++)
		free(argv[i]);
	return (0);
}

/**
 * tokenize_command - Tokenizes the command string
 * @command: The command string
 * @argv: Array to hold the tokenized arguments
 *
 * Return: Number of tokens
 */
int tokenize_command(char *command, char *argv[])
{
	char **tokens;
	int i = 0, j = 0;

	tokens = tokenize(command, ' ');
	if (tokens == NULL || argv == NULL)
		return (0);
	while (tokens[i] != NULL && i < 99)
	{
		argv[i] = strdup(tokens[i]);
		if (argv[i] == NULL)
		{
			for (j = 0; j < i; j++)
				free(argv[j]);
			free_tokens(tokens);
			return (0);
		}
		i++;
	}
	argv[i] = NULL;

	free_tokens(tokens);  /* Free the allocated memory for tokens */
	return (i);
}

/**
 * execute_command - Executes a command entered by the user
 * @argv: The array of arguments
 *
 * Description: Uses execve to execute the command. Prints an error message
 * if the command is not found.
 */
void execute_command(char **argv)
{
	pid_t pid;
	int status;

	if (argv[0] == NULL) /* Check for empty command */
		return;
	pid = fork();
	if (pid == 0) /* Child process */
	{
		if (execve(argv[0], argv, environ) == -1)
			handle_error();
		exit(EXIT_FAILURE);
	}
	else /* Parent process */
		wait(&status);
}

/**
 * print_env - Prints the current environment
 *
 * Description: Iterates through the environment variables and prints each one.
 */
void print_env(void)
{
	int i = 0;

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * handle_error - Handles errors by printing an error message
 */
void handle_error(void)
{
	write(STDERR_FILENO, "./hsh: ", 7);
	write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
	write(STDERR_FILENO, "\n", 1);
}

