#include "main.h"
#include <signal.h>

/* Global pointer for command buffer */
char *command_buffer = NULL;

/**
 * sigint_handler - Handles SIGINT signal (Ctrl+C)
 * @sig: Signal number
 */
void sigint_handler(int sig)
{
	(void)sig; /* Unused parameter */

	/* Free allocated memory */
	if (command_buffer != NULL)
	{
		free(command_buffer);
		command_buffer = NULL;
	}

	/* Print a new prompt */
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
		command_buffer = NULL;
	}
}

