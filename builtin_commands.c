#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


/**
 * echo_cmd - Handles the `echo` command,
 * including environment variable expansion.
 * @argv: Array of arguments
 */
void echo_cmd(char **argv)
{
	char *processed_string;
	int i = 1;

	if (argv[i] == NULL)
	{
		write(STDOUT_FILENO, "\n", 1);
		return;
	}

	while (argv[i])
	{
		processed_string = replace_env_var(argv[i]);
		if (processed_string)
		{
			write(STDOUT_FILENO, processed_string, strlen(processed_string));
			if (argv[i + 1])
				write(STDOUT_FILENO, " ", 1);
			free(processed_string);
		}
		else
		{
			write(STDOUT_FILENO, "echo: memory allocation failed\n", 31);
			return;
		}
		i++;
	}

	write(STDOUT_FILENO, "\n", 1);
}
/**
 * cd_cmd - Changes the current directory of the process
 * @argv: Array of arguments (should be "DIRECTORY" or "cd -")
 */
void cd_cmd(char **argv)
{
	char *home_dir = getenv("HOME");
	char *prev_dir = getenv("OLDPWD");
	char current_dir[1024];
	int result;

	if (argv[1] == NULL) /* No argument given, change to $HOME */
	{
		if (home_dir == NULL)
		{
			write(STDERR_FILENO, "HOME environment variable not set\n", 35);
			return;
		}
		result = chdir(home_dir);
	}
	else if (strcmp(argv[1], "-") == 0) /* Change to the previous directory */
	{
		if (prev_dir == NULL)
		{
			write(STDERR_FILENO, "OLDPWD environment variable not set\n", 35);
			return;
		}
		result = chdir(prev_dir);
	}
	else /* Change to the specified directory */
		result = chdir(argv[1]);
	if (result == -1)
	{
		perror("cd");
		return;
	}
	if (getcwd(current_dir, sizeof(current_dir)) != NULL)
	{
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", current_dir, 1);
	}
	else
		perror("getcwd");
}

/**
 * pwd - Prints the current working directory
 */
void pwd(void)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(STDOUT_FILENO, cwd, strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		perror("pwd");
	}
}

