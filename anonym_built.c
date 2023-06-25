#include "shell.h"

int shello_alias(char **args, char __attribute__((__unused__)) **front);
void set_alias(char *var_name, char *value);
void print_alias(alias_t *alias);

/**
 * shello_alias - Builtin comm which  print all aliase, specific
 * alias, or sets an alias.
 * @args: Array of argument.
 * @front: Double pointer to the start of args.
 *
 * Return: If error occurs - -1.
 *         Otherwise - 0.
 */

int shello_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *farr = aliases;
	int a, rot = 0;
	char *value;

	if (!args[0])
	{
		while (farr)
		{
			print_alias(farr);
			farr = farr->next;
		}
		return (rot);
	}
	for (a = 0; args[a]; a++)
	{
		farr = aliases;
		value = _strchr(args[a], '=');
		if (!value)
		{
			while (farr)
			{
				if (_strcmp(args[a], farr->name) == 0)
				{
					print_alias(farr);
					break;
				}
				farr = farr->next;
			}
			if (!farr)
				rot = make_error(args + a, 1);
		}
		else
			set_alias(args[a], value);
	}
	return (rot);
}

/**
 * set_alias - Should set existing alias 'name' with recent val,
 * 'value' or make a new alias with 'name' and 'value'.
 * @var_name: Name of alias.
 * @value: Value of alias. First char is a '='.
 */

void set_alias(char *var_name, char *value)
{
	alias_t *farr = aliases;
	int len, l, m;
	char *nw_value;

	*value = '\0';
	value++;
	len = _strlen(value) - _strspn(value, "'\"");
	nw_value = malloc(sizeof(char) * (len + 1));
	if (!nw_value)
		return;
	for (l = 0, m = 0; value[l]; m++)
	{
		if (value[l] != '\'' && value[l] != '"')
			nw_value[m++] = value[l];
	}
	nw_value[m] = '\0';
	while (farr)
	{
		if (_strcmp(var_name, farr->name) == 0)
		{
			free(farr->value);
			farr->value = nw_value;
			break;
		}
		farr = farr->next;
	}
	if (!farr)
		add_alias(&aliases, var_name, nw_value);
}

/**
 * print_alias - Print d alias in format name='val'.
 * @alias: Pointer to alias.
 */

void print_alias(alias_t *alias)
{
	char *alias_str;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_str = malloc(sizeof(char) * (len + 1));
	if (!alias_str)
		return;
	_strcpy(alias_str, alias->name);
	_strcat(alias_str, "='");
	_strcat(alias_str, alias->value);
	_strcat(alias_str, "'\n");

	write(STDOUT_FILENO, alias_str, len);
	free(alias_str);
}
/**
 * restore_aliases - Pass thru arguments and replaces any matching alias
 * with their value.
 * @args: 3D pointer to d arguments.
 * Return: 3D pointer to d arguments.
 */

char **restore_aliases(char **args)
{
	alias_t *farr;
	int a;
	char *nw_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (a = 0; args[a]; a++)
	{
		farr = aliases;
		while (farr)
		{
			if (_strcmp(args[a], farr->name) == 0)
			{
				nw_value = malloc(sizeof(char) * (_strlen(farr->value) + 1));
				if (!nw_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(nw_value, farr->value);
				free(args[a]);
				args[a] = nw_value;
				a--;
				break;
			}
			farr = farr->next;
		}
	}

	return (args);
}

