#include "main.h"

/**
 * setenv_cmd - Initializes a new environment variable,
 * or modifies an existing one
 * @argv: Array of arguments (should be "VARIABLE VALUE")
 */
void setenv_cmd(char **argv)
{
	if (argv[1] == NULL || argv[2] == NULL)
	{
		write(STDERR_FILENO, "Usage: setenv VARIABLE VALUE\n", 30);
		return;
	}

	if (setenv(argv[1], argv[2], 1) == -1)
	{
		handle_error();
	}
}

/**
 * unsetenv_cmd - Removes an environment variable
 * @argv: Array of arguments (should be "VARIABLE")
 */
void unsetenv_cmd(char **argv)
{
	if (argv[1] == NULL)
	{
		write(STDERR_FILENO, "Usage: unsetenv VARIABLE\n", 26);
		return;
	}

	if (unsetenv(argv[1]) == -1)
	{
		handle_error();
	}
}

