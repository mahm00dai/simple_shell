#include "main.h"

/**
 * main - Entry point for the shell program
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	simple_shell();
	return (0);
}

/**
 * simple_shell - The core function of the shell
 *
 * Description: Displays a prompt, reads a command, and executes it.
 * Continues to prompt until "Ctrl+D" or "exit" is encountered.
 */
void simple_shell(void)
{
	char *command;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		command = read_command();

		if (command == NULL) /* Handle Ctrl+D (EOF) */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (process_command(command) == -1)
			continue;
	}
	free(command);
}

