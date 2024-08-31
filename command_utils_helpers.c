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
 * free_tokens - Frees the memory allocated for the tokens.
 * @tokens: Array of strings (tokens) to be freed.
 */
void free_tokens(char **tokens)
{
	size_t i;

	if (tokens == NULL)
		return;

	for (i = 0; tokens[i] != NULL; i++)
		free(tokens[i]);
	free(tokens);
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
	size_t num_tokens, i = 0, j = 0, token_len = 0;
	char **tokens = NULL;
	const char *start = NULL;

	if (str == NULL)
		return (NULL);

	num_tokens = count_tokens(str, delim);
	tokens = malloc((num_tokens + 1) * sizeof(char *));
	if (tokens == NULL)
		return (NULL);

	while (*str)
	{
		if (*str != delim && start == NULL)
			start = str;  /* Mark the start of the token */
		else if ((*str == delim || *(str + 1) == '\0') && start != NULL)
		{
			if (*str == delim)
				token_len = str - start;
			else
				token_len = str - start + 1;
			tokens[i] = copy_token(start, token_len);
			if (tokens[i] == NULL)
			{
				for (j = 0; j < i; j++)
					free(tokens[j]);
				free(tokens);
				return (NULL);
			}
			i++;
			start = NULL;
		}
		str++;
	}
	tokens[i] = NULL;  /* Null-terminate the array of tokens */

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

