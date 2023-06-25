#include "shell.h"
int (*get_builtin(char *command))(char **args, char **front);
int shello_exit(char **args, char **front);
int shello_cd(char **args, char __attribute__((__unused__)) **front);
int shello_help(char **args, char __attribute__((__unused__)) **front);

/**
 * get_builtin - Ties a command with a similar
 *               shello builtin func.
 * @command: Command to match.
 *
 * Return: Function pointer which correspond to the builtin.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", shello_exit },
		{ "env", shello_env },
		{ "setenv", shello_setenv },
		{ "unsetenv", shello_unsetenv },
		{ "cd", shello_cd },
		{ "alias", shello_alias },
		{ "help", shello_help },
		{ NULL, NULL }
	};
	int z;

	for (z = 0; funcs[z].name; z++)
	{
		if (_strcmp(funcs[z].name, command) == 0)
			break;
	}
	return (funcs[z].f);
}

/**
 * shello_exit - Allow normal process halt
 *                for the shello shell.
 * @args: Array of arguments housing the exit value.
 * @front: Double pointer to the beginning of args.
 *
 * Return: Where there are no arguments - -3.
 *         Where the given exit value is invalid - 2.
 *         O/w - exits with the given status value.
 *
 * Description: When returning -3, the program exits in the main function.
 */
int shello_exit(char **args, char **front)
{
	int z, length_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			z = 1;
			length_of_int++;
		}
		for (; args[0][z]; z++)
		{
			if (z <= length_of_int && args[0][z] >= '0' && args[0][z] <= '9')
				num = (num * 10) + (args[0][z] - '0');
			else
				return (make_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (make_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(num);
}

/**
 * shello_cd - Change the current directory of the shello process.
 * @args: Array of arguments.
 * @front: Double pointer to the start of args.
 *
 * Return: Where the given str is not a directory - 2.
 *         Where an error occurs - -1.
 *         Otherwise - 0.
 */
int shello_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info, *new_num = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (_goenv("OLDPWD") != NULL)
					(chdir(*_goenv("OLDPWD") + 7));
			}
			else
			{
				free(oldpwd);
				return (make_error(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldpwd);
				return (make_error(args, 2));
			}
		}
	}
	else
	{
		if (_goenv("HOME") != NULL)
			chdir(*(_goenv("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	dir_info = malloc(sizeof(char *) * 2);
	if (!dir_info)
		return (-1);

	dir_info[0] = "OLDPWD";
	dir_info[1] = oldpwd;
	if (shello_setenv(dir_info, dir_info) == -1)
		return (-1);

	dir_info[0] = "PWD";
	dir_info[1] = pwd;
	if (shello_setenv(dir_info, dir_info) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, new_num, 1);
	}
	free(oldpwd);
	free(pwd);
	free(dir_info);
	return (0);
}

/**
 * shello_help - Shows info abt shello builtin command.
 * @args: Array of args.
 * @front: Pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shello_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		aid_all();
	else if (_strcmp(args[0], "alias") == 0)
		aid_alias();
	else if (_strcmp(args[0], "cd") == 0)
		aid_cd();
	else if (_strcmp(args[0], "exit") == 0)
		aid_exit();
	else if (_strcmp(args[0], "env") == 0)
		aid_env();
	else if (_strcmp(args[0], "setenv") == 0)
		aid_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		aid_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		aid_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}
