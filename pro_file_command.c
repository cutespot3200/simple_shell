#include "shell.h"

int dont_open(char *file_path);
int proc_file_command(char *file_path, int *exe_ret);

/**
 * dont_open - Where a file doesn't exist or lacks proper permissions, print
 * a dont open error.
 * @file_path: Paths to supposed file.
 *
 * Return: 127.
 */

int dont_open(char *file_path)
{
	char *error, *hist_str;
	int len;

	hist_str = _ita(hist);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Dan't open ");
	_strcat(error, file_path);
	_strcat(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, len);
	free(error);
	return (127);
}

/**
 * proc_file_command - Takes a file and attempts to run the commands stored
 * within.
 * @file_path: Paths to file.
 * @exe_ret: Return val of the last executed command.
 *
 * Return: Where file couldn't be opened - 127.
 *	   If malloc fails - -1.
 *	   Otherwise return val of the last command ran.
 */
int proc_file_command(char *file_path, int *exe_ret)
{
	ssize_t file, b_read, l;
	unsigned int line_size = 0;
	unsigned int old_size = 120;
	char *line, **args, **front;
	char buffer[120];
	int rot;

	hist = 0;
	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		*exe_ret = dont_open(file_path);
		return (*exe_ret);
	}
	line = malloc(sizeof(char) * old_size);
	if (!line)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && line_size == 0)
			return (*exe_ret);
		buffer[b_read] = '\0';
		line_size += b_read;
		line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read);
	for (l = 0; line[l] == '\n'; l++)
		line[l] = ' ';
	for (; l < line_size; l++)
	{
		if (line[l] == '\n')
		{
			line[l] = ';';
			for (l += 1; l < line_size && line[l] == '\n'; l++)
				line[l] = ' ';
		}
	}
	vari_rep(&line, exe_ret);
	hold_line(&line, line_size);
	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (0);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (l = 0; args[l]; l++)
	{
		if (_strncmp(args[l], ";", 1) == 0)
		{
			free(args[l]);
			args[l] = NULL;
			rot = dub_args(args, front, exe_ret);
			args = &args[++l];
			l = 0;
		}
	}

	rot = dub_args(args, front, exe_ret);

	free(front);
	return (rot);
}
