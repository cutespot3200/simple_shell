#include "shell.h"

char **_dupenv(void);
void free_env(void);
char **_goenv(const char *var);

/**
 * _dupenv - Makes a duplicate of the environment.
 *
 * Return: Where an error occurs - NULL.
 *         O/w - a double pointer to the new copy.
 */
char **_dupenv(void)
{
	char **new_env;
	size_t size;
	int ind;

	for (size = 0; environ[size]; size++)
		;

	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);

	for (ind = 0; environ[ind]; ind++)
	{
		new_env[ind] = malloc(_strlen(environ[ind]) + 1);

		if (!new_env[ind])
		{
			for (ind--; ind >= 0; ind--)
				free(new_env[ind]);
			free(new_env);
			return (NULL);
		}
		_strcpy(new_env[ind], environ[ind]);
	}
	new_env[ind] = NULL;

	return (new_env);
}

/**
 * free_env - Frees the env copy.
 */
void free_env(void)
{
	int ind;

	for (ind = 0; environ[ind]; ind++)
		free(environ[ind]);
	free(environ);
}

/**
 * _goenv - Gets  environmental var from the PATH.
 * @var: Name of environmental variable to get.
 *
 * Return: Where the environmental variable does not exist - NULL.
 *         Otherwise - a pointer to the environmental variable.
 */
char **_goenv(const char *var)
{
	int ind, len;

	len = _strlen(var);
	for (ind = 0; environ[ind]; ind++)
	{
		if (_strncmp(var, environ[ind], len) == 0)
			return (&environ[ind]);
	}

	return (NULL);
}
