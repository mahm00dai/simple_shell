#include "main.h"
#include <stdlib.h> /* For malloc and free */
#include <stdio.h>  /* For stdin */
#include <string.h> /* For strlen */
#include <unistd.h> /* For read */

/**
 * trim_leading_spaces - Trims leading spaces from a string
 * @str: The string to be trimmed
 *
 * Return: A pointer to the trimmed string (may be a new allocation)
 */
static char *trim_leading_spaces(char *str)
{
	char *temp;
	size_t i;

	for (i = 0; str[i] == ' '; i++)
		;

	if (i > 0)
	{
		temp = malloc(strlen(str) - i + 1);
		if (temp == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strcpy(temp, str + i);
		free(str);
		str = temp;
	}

	return (str);
}

/**
 * _getline - Custom implementation of getline function
 * @lineptr: Pointer to buffer where the line will be stored
 * @n: Pointer to the size of the buffer
 * @stream: Input stream (usually stdin)
 *
 * Return: Number of characters read, or -1 on failure or EOF
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static char buffer[1024];
	size_t pos = 0, total_read = 0;
	ssize_t bytes_read = 0;
	char *new_lineptr;

	if (lineptr == NULL || n == NULL || stream == NULL)
		return (-1);
	if (*lineptr == NULL)
	{
		*n = 1024;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}
	while (1)
	{
		if (pos >= (size_t)bytes_read)
		{
			bytes_read = read(fileno(stream), buffer, sizeof(buffer));
			if (bytes_read <= 0)
				return (total_read > 0 ? (ssize_t)total_read : -1);
			pos = 0;
		}
		while (pos < (size_t)bytes_read && total_read < *n - 1)
		{
			(*lineptr)[total_read++] = buffer[pos++];
			if ((*lineptr)[total_read - 1] == '\n')
			{
				(*lineptr)[total_read] = '\0';
				return ((ssize_t)total_read);
			}
		}
		if (total_read >= *n - 1)
		{
			*n *= 2;
			new_lineptr = realloc(*lineptr, *n);
			if (new_lineptr == NULL)
				return (-1);
			*lineptr = new_lineptr;
		}
	}
}

/**
 * read_command - Reads a command from standard input using the custom _getline
 *
 * Return: A pointer to the command string
 */
char *read_command(void)
{
	char *buffer;
	char *trimmed;
	size_t bufsize = 1024;
	ssize_t nread;

	buffer = malloc(bufsize);
	if (buffer == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	/* Read the command from stdin using the custom _getline */
	nread = _getline(&buffer, &bufsize, stdin);

	if (nread == -1) /* Handle EOF or read error */
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}

	/* Remove newline character if present */
	if (nread > 0 && buffer[nread - 1] == '\n')
		buffer[nread - 1] = '\0';

	/* Trim leading spaces */
	trimmed = trim_leading_spaces(buffer);

	if (trimmed != buffer)
	{
		free(buffer);
		buffer = trimmed;
	}
	return (buffer);
}

