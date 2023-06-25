#include "shell.h"

int shello_env(char **args, char __attribute__((__unused__)) **front);
int shello_setenv(char **args, char __attribute__((__unused__)) **front);
int shello_unsetenv(char **args, char __attribute__((__unused__)) **front);

/**
 * shello_env - Prints the curr environ.
 * @args: Array of arguments passed to the shell.
 * @front: Double pointer to the beginning of args.
 *
 * Return: Where error occurs - -1.
 *	   Otherwise - 0.
 *
 * Description: Prints one variable per line in the
 *              format 'variable'='value'.
 */
int shello_env(char **args, char __attribute__((__unused__)) **front)
{
	int ind;
	char nc = '\n';

	if (!environ)
		return (-1);

	for (ind = 0; environ[ind]; ind++)
	{
		write(STDOUT_FILENO, environ[ind], _strlen(environ[ind]));
		write(STDOUT_FILENO, &nc, 1);
	}

	(void)args;
	return (0);
}

/**
 * shello_setenv - Changes or adds an environ var to the PATH.
 * @args: Array of args passed to the shell.
 * @front: Double pointer to the beginning of args.
 * Description: args[1] is the name of the new or existing PATH variable.
 *              args[2] is the value to set the new or changed variable to.
 *
 * Return: Where an error occurs - -1.
 *         Otherwise - 0.
 */
int shello_setenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var = NULL, **new_env, *new_val;
	size_t size;
	int ind;

	if (!args[0] || !args[1])
		return (make_error(args, -1));

	new_val = malloc(_strlen(args[0]) + 1 + _strlen(args[1]) + 1);
	if (!new_val)
		return (make_error(args, -1));
	_strcpy(new_val, args[0]);
	_strcat(new_val, "=");
	_strcat(new_val, args[1]);

	env_var = _goenv(args[0]);
	if (env_var)
	{
		free(*env_var);
		*env_var = new_val;
		return (0);
	}
	for (size = 0; environ[size]; size++)
		;

	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
	{
		free(new_val);
		return (make_error(args, -1));
	}

	for (ind = 0; environ[ind]; ind++)
		new_env[ind] = environ[ind];

	free(environ);
	environ = new_env;
	environ[ind] = new_val;
	environ[ind + 1] = NULL;

	return (0);
}

/**
 * shello_unsetenv - Remove environmental variable from the PATH.
 * @args: Array of args passed to the shell.
 * @front: Double pointer to the beginning of args.
 * Description: args[1] is the PATH variable to remove.
 *
 * Return: Where an error occurs - -1.
 *         Otherwise - 0.
 */
int shello_unsetenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var, **new_env;
	size_t size;
	int ind, ind2;

	if (!args[0])
		return (make_error(args, -1));
	env_var = _goenv(args[0]);
	if (!env_var)
		return (0);

	for (size = 0; environ[size]; size++)
		;

	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (make_error(args, -1));

	for (ind = 0, ind2 = 0; environ[ind]; ind++)
	{
		if (*env_var == environ[ind])
		{
			free(*env_var);
			continue;
		}
		new_env[ind2] = environ[ind];
		ind2++;
	}
	free(environ);
	environ = new_env;
	environ[size - 1] = NULL;

	return (0);
}
