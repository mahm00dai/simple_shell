#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * get_env_var_value - Retrieves the value of an environment variable
 * @env_var: Environment variable name
 *
 * Return: Value of the environment variable or NULL if not found
 */
char *get_env_var_value(const char *env_var)
{
	return (getenv(env_var));
}

/**
 * append_env_value - Appends environment variable value to result string
 * @result: Current result string
 * @ptr: Pointer to the end of the result string
 * @env_value: Environment variable value
 *
 * Return: Updated result string or NULL on failure
 */
char *append_env_value(char *result, char *ptr, const char *env_value)
{
	size_t env_value_len = strlen(env_value);
	char *new_result;

	if (ptr + env_value_len > result + strlen(result))
	{
		new_result = realloc(result, strlen(result) + env_value_len + 1);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		result = new_result;
		ptr = result + (ptr - result);
	}
	strcpy(ptr, env_value);
	return (result);
}

/**
 * extract_env_var - Extracts the environment variable name from the string
 * @str: Pointer to the string starting with the env variable
 * @var_len: Pointer to store the length of the variable name
 *
 * Return: Dynamically allocated string containing the env variable name
 */
char *extract_env_var(const char **str, size_t *var_len)
{
	const char *env_var_start = *str + 1;

	while (**str && **str != ' ' && **str != '$')
		(*str)++;
	*var_len = *str - env_var_start;

	return (strndup(env_var_start, *var_len));
}

/**
 * handle_env_var - Processes an environment variable and appends its value
 * @result: Pointer to the result string
 * @ptr: Pointer to the end of the result string
 * @str: Pointer to the string containing the env variable
 *
 * Return: Updated pointer to the result string or NULL on failure
 */
char *handle_env_var(char *result, char **ptr, const char **str)
{
	size_t var_len;
	char *env_var_value = extract_env_var(str, &var_len);
	char *env_value;

	if (!env_var_value)
	{
		free(result);
		return (NULL);
	}

	env_value = get_env_var_value(env_var_value);
	free(env_var_value);

	if (env_value)
	{
		result = append_env_value(result, *ptr, env_value);
		if (!result)
			return (NULL);
		*ptr += strlen(env_value);
	}
	else
	{
		**ptr = '\0';
		(*ptr)++;
	}

	return (result);
}

/**
 * replace_env_var - Replaces environment variable
 * placeholders with their values
 * @str: The string to process
 *
 * Return: A new string with environment variables replaced or NULL on failure
 */
char *replace_env_var(const char *str)
{
	size_t len = strlen(str);
	char *result = malloc(len * 2 + 1);
	char *ptr = result;

	if (!result)
		return (NULL);

	while (*str)
	{
		if (*str == '$' && *(str + 1) != '\0' && *(str + 1) != ' ')
		{
			result = handle_env_var(result, &ptr, &str);
			if (!result)
				return (NULL);
		}
		else
		{
			*ptr++ = *str++;
		}
	}
	*ptr = '\0';

	return (result);
}

