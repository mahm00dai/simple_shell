#include "main.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * sigint_handler - Handles SIGINT signal (Ctrl+C)
 * @sig: Signal number
 */
void sigint_handler(int sig)
{
	(void)sig; /* Unused parameter */

	/* Print a new prompt and exit */
	write(STDOUT_FILENO, "\n", 1);
	exit(0);
}

/**
 * main - Entry point for the shell program
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	/* Register the signal handler */
	signal(SIGINT, sigint_handler);

	/* Start the shell */
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
	char *command_buffer = NULL;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		/* Read command using custom _getline */
		command_buffer = read_command();

		if (command_buffer == NULL) /* Handle Ctrl+D (EOF) */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (process_command(command_buffer) == -1)
			continue;

		/* Free the command buffer after processing */
		free(command_buffer);
		command_buffer = NULL;
	}

	if (command_buffer != NULL)
	{
		free(command_buffer);
	}
}

