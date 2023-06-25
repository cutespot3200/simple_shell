#include "shell.h"


int token_loc(char *str, char *delim);
int total_token(char *str, char *delim);
char **_strtok(char *line, char *delim);

/**
 * token_loc - Locate delim index marking the end
 *             of the first token contained within a str.
 * @str: Str to be searched.
 * @delim: Delimiter character.
 *
 * Return: Delimiter ind marking the end of
 *         the intitial token pointed to be str.
 */
int token_loc(char *str, char *delim)
{
	int ind = 0, len = 0;

	while (*(str + ind) && *(str + ind) != *delim)
	{
		len++;
		ind++;
	}

	return (len);
}

/**
 * total_token - Include no of delim
 *                words enclosed within a str.
 * @str: String to be searched.
 * @delim: Delimiter char.
 *
 * Return: Number of words contained within str.
 */
int total_token(char *str, char *delim)
{
	int ind, token = 0, len = 0;

	for (ind = 0; *(str + ind); ind++)
		len++;

	for (ind = 0; ind < len; ind++)
	{
		if (*(str + ind) != *delim)
		{
			token++;
			ind += token_loc(str + ind, delim);
		}
	}

	return (token);
}

/**
 * _strtok - Tokenises a str.
 * @line: The str.
 * @delim: Delimiter char to tokenise the str.
 *
 * Return: Pointer to array containing the tokenised words.
 */
char **_strtok(char *line, char *delim)
{
	char **ptr;
	int ind = 0, token, t, alpha, i;

	token = total_token(line, delim);
	if (token == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (token + 2));
	if (!ptr)
		return (NULL);

	for (t = 0; t < token; t++)
	{
		while (line[ind] == *delim)
			ind++;

		alpha = token_loc(line + ind, delim);

		ptr[t] = malloc(sizeof(char) * (alpha + 1));
		if (!ptr[t])
		{
			for (ind -= 1; ind >= 0; ind--)
				free(ptr[ind]);
			free(ptr);
			return (NULL);
		}

		for (i = 0; i < alpha; i++)
		{
			ptr[t][i] = line[ind];
			ind++;
		}

		ptr[t][i] = '\0';
	}
	ptr[t] = NULL;
	ptr[t + 1] = NULL;

	return (ptr);
}
