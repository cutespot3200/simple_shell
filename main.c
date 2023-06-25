#include "shell.h"

void signa_handle(int sig);
int execute(char **args, char **front);

/**
 * signa_handle - Prints a new prompt when signalled.
 * @sig: Signal.
 */
void signa_handle(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, signa_handle);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * implement - Executes a command in a process.
 * @args: Array of args.
 * @front: Double pointer to the beginning of args.
 *
 * Return: Where error occur - a corresponding error code.
 *         O/w - Exit value of the last executed command.
 */
int implement(char **args, char **front)
{
	pid_t child_pid;
	int stat, fag = 0, rot = 0;
	char *command = args[0];

	if (command[0] != '/' && command[0] != '.')
	{
		fag = 1;
		command = go_loc(command);
	}

	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			rot = (make_error(args, 126));
		else
			rot = (make_error(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (fag)
				free(command);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(command, args, environ);
			if (errno == EACCES)
				rot = (make_error(args, 126));
			free_env();
			free_args(args, front);
			free_alias_list(aliases);
			_exit(rot);
		}
		else
		{
			wait(&stat);
			rot = WEXITSTATUS(stat);
		}
	}
	if (fag)
		free(command);
	return (rot);
}

/**
 * main - Run a simple UNIX command interpreter.
 * @argc: Number of args supplied to the program.
 * @argv: Array of pointers to the arguments.
 *
 * Return: Return value of the last executed command.
 */
int main(int argc, char *argv[])
{
	int rot = 0, rotn;
	int *exe_ret = &rotn;
	char *prompt = "$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, signa_handle);

	*exe_ret = 0;
	environ = _dupenv();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		rot = proc_file_command(argv[1], exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (rot != END_OF_FILE && rot != EXIT)
			rot = hold_args(exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}

	while (1)
	{
		write(STDOUT_FILENO, prompt, 2);
		rot = hold_args(exe_ret);
		if (rot == END_OF_FILE || rot == EXIT)
		{
			if (rot == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_env();
			free_alias_list(aliases);
			exit(*exe_ret);
		}
	}

	free_env();
	free_alias_list(aliases);
	return (*exe_ret);
}
