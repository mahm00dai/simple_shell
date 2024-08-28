#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * replace_env_var - Replaces environment variable
 * placeholders with their values.
 * @str: The string to process.
 *
 * Return: A new string with environment variables replaced,
 * or NULL on failure.
 */
char *replace_env_var(const char *str)
{
	size_t len = strlen(str);
	char *result = malloc(len + 1);
	char *ptr = result;
	const char *env_var_start;
	size_t var_len;
	char *env_var_value;

	if (!result)
		return (NULL);

	while (*str)
	{
		if (*str == '$' && *(str + 1) != '\0' && *(str + 1) != ' ')
		{
			env_var_start = str + 1;

			while (*str && *str != ' ' && *str != '$')
				str++;

			var_len = str - env_var_start;
			env_var_value = malloc(var_len + 1);
			if (!env_var_value)
			{
				free(result);
				return (NULL);
			}

			strncpy(env_var_value, env_var_start, var_len);
			env_var_value[var_len] = '\0';

			char *env_value = getenv(env_var_value);

			if (env_value)
				strcpy(ptr, env_value);
			else
				strcpy(ptr, "");

			ptr += strlen(ptr);
			free(env_var_value);
		}
		else
		{
			*ptr++ = *str++;
		}
	}

	*ptr = '\0';
	return (result);
}
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
			handle_error("echo: memory allocation failed");
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

