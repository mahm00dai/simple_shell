#include "main.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
/**
 * count_tokens - Counts the number of tokens in the string.
 * @str: The string to be tokenized.
 * @delim: The delimiter character.
 *
 * Return: The number of tokens.
 */
size_t count_tokens(const char *str, char delim)
{
	size_t num_tokens = 0;
	size_t i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == delim)
		{
			if (i > 0 && str[i - 1] != delim)
				num_tokens++;
		}
	}
	if (i > 0 && str[i - 1] != delim)
		num_tokens++;
	return (num_tokens);
}

/**
 * copy_token - Copies a token from the string.
 * @start: Pointer to the start of the token.
 * @length: Length of the token.
 *
 * Return: Pointer to the newly allocated token.
 */
char *copy_token(const char *start, size_t length)
{
	char *token;
	size_t i;

	token = (char *)malloc(length + 1);
	if (token)
	{
		for (i = 0; i < length; i++)
			token[i] = start[i];
		token[length] = '\0';
	}
	return (token);
}

/**
 * tokenize - Splits a string into tokens based on a delimiter.
 * @str: The string to be tokenized.
 * @delim: The delimiter character.
 *
 * Return: An array of strings (tokens), or NULL on failure.
 */
char **tokenize(char *str, char delim)
{
	size_t num_tokens, i = 0, j = 0;
	char **tokens = NULL;
	char *token_start = NULL;
	size_t token_length = 0;

	if (!str)
		return (NULL);
	num_tokens = count_tokens(str, delim);

	tokens = (char **)malloc((num_tokens + 2) * sizeof(char *));
	if (!tokens)
		return (NULL);
	while (str[i])
	{
		while (str[i] == delim)
			i++;
		if (str[i] == '\0')
			break;
		token_start = &str[i];
		token_length = 0;

		while (str[i] && str[i] != delim)
		{
			i++;
			token_length++;
		}
		tokens[j] = copy_token(token_start, token_length);
		if (!tokens[j])
		{
			for (size_t k = 0; k < j; k++)
				free(tokens[k]);
			free(tokens);
			return (NULL);
		}
		j++;
	}
	tokens[j] = NULL;

	return (tokens);
}

/**
 * exit_shell - Exits the shell with a specified status
 * @status: The exit status to use when exiting the shell
 */
void exit_shell(int status)
{
	/* Convert status to a valid exit status (0-255) */
	status = status & 0xFF;
	exit(status);
}

