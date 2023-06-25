#include "shell.h"

char *go_args(char *line, int *exe_ret);
int dub_args(char **args, char **front, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int hold_args(int *exe_ret);
int check_args(char **args);

/**
 * go_args - Takes a command from stdin.
 * @line: Buffer to store the command.
 * @exe_ret: Return val of last executed command.
 *
 * Return: Where error occur - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *go_args(char *line, int *exe_ret)
{
	size_t t = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = get_line(&line, &t, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (go_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	vari_rep(&line, exe_ret);
	hold_line(&line, read);

	return (line);
}

/**
 * dub_args - Divides operators from commands and dubs them.
 * @args: Array of args.
 * @front: Double pointer to the beginning of args.
 * @exe_ret: Return val of the parent process' last executed command.
 *
 * Return: Return value of the last executed command.
 */
int dub_args(char **args, char **front, int *exe_ret)
{
	int rot, ind;

	if (!args[0])
		return (*exe_ret);
	for (ind = 0; args[ind]; ind++)
	{
		if (_strncmp(args[ind], "||", 2) == 0)
		{
			free(args[ind]);
			args[ind] = NULL;
			args = restore_aliases(args);
			rot = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++ind];
				ind = 0;
			}
			else
			{
				for (ind++; args[ind]; ind++)
					free(args[ind]);
				return (rot);
			}
		}
		else if (_strncmp(args[ind], "&&", 2) == 0)
		{
			free(args[ind]);
			args[ind] = NULL;
			args = restore_aliases(args);
			rot = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++ind];
				ind = 0;
			}
			else
			{
				for (ind++; args[ind]; ind++)
					free(args[ind]);
				return (rot);
			}
		}
	}
	args = restore_aliases(args);
	rot = run_args(args, front, exe_ret);
	return (rot);
}

/**
 * run_args - Dubs execution of a command.
 * @args: Array of args.
 * @front: Double pointer to the beginning of args.
 * @exe_ret: Return val of the parent process' last executed command.
 *
 * Return: Return val of the last executed command.
 */
int run_args(char **args, char **front, int *exe_ret)
{
	int rot, l;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		rot = builtin(args + 1, front);
		if (rot != EXIT)
			*exe_ret = rot;
	}
	else
	{
		*exe_ret = implement(args, front);
		rot = *exe_ret;
	}

	hist++;

	for (l = 0; args[l]; l++)
		free(args[l]);

	return (rot);
}

/**
 * hold_args - Takes, dubs, and runs execution of a command.
 * @exe_ret: Return val of  parent process' last executed command.
 *
 * Return: Where end-of-file is read - END_OF_FILE (-2).
 *         Where input cannot be tokenized - -1.
 *         O/w  val of last executed command.
 */
int hold_args(int *exe_ret)
{
	int rot = 0, ind;
	char **args, *line = NULL, **front;

	line = go_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (rot);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (ind = 0; args[ind]; ind++)
	{
		if (_strncmp(args[ind], ";", 1) == 0)
		{
			free(args[ind]);
			args[ind] = NULL;
			rot = dub_args(args, front, exe_ret);
			args = &args[++ind];
			ind = 0;
		}
	}
	if (args)
		rot = dub_args(args, front, exe_ret);

	free(front);
	return (rot);
}

/**
 * check_args - Check if there are  leading ';', ';;', '&&', or '||'.
 * @args: Pointer to tokenized commands and arguments.
 *
 * Return: Where  ';', '&&', or '||' is placed at an invalid position - 2.
 *	   Otherwise - 0.
 */
int check_args(char **args)
{
	size_t l;
	char *cur, *nex;

	for (l = 0; args[l]; l++)
	{
		cur = args[l];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (l == 0 || cur[1] == ';')
				return (make_error(&args[l], 2));
			nex = args[l + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (make_error(&args[l + 1], 2));
		}
	}
	return (0);
}
