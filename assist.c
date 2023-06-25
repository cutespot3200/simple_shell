#include "shell.h"

void free_args(char **args, char **front);
char *get_pid(void);
char *get_env_value(char *beginning, int len);
void vari_rep(char **args, int *exe_ret);

/**
 * free_args - Releases memory taken by args.
 * @args: Null-terminated double ptr containing commands/args.
 * @front: Double pointer to the beginning of args.
 */
void free_args(char **args, char **front)
{
	size_t z;

	for (z = 0; args[z] || args[z + 1]; z++)
		free(args[z]);

	free(front);
}

/**
 * get_pid - Takescurrent process ID.
 * Description: Opens the stat file, a space-delimited file containing
 *              information about the current process.
 *
 * Return: Current process ID or NULL on failure.
 */
char *get_pid(void)
{
	size_t z = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[z] != ' ')
		z++;
	buffer[z] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Takes val corresponding to environmental var.
 * @beginning: Environmental var to search for.
 * @len: Length of environmental var to search for.
 *
 * Return: Where the variable is not found - an empty string.
 *         Otherwise - the value of the environmental variable.
 *
 * Description: Variables are stored in the format VARIABLE=VALUE.
 */
char *get_env_value(char *beginning, int len)
{
	char **var_addr;
	char *replacement = NULL, *temp, *var;

	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_addr = _goenv(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (replacement)
			_strcpy(replacement, temp);
	}

	return (replacement);
}

/**
 * vari_rep - Takes care of variable replacement.
 * @line: Double pointer having the command and args.
 * @exe_ret: Pointer to return val of last executed command.
 *
 * Description: Replaces $$ with the current PID, $? with the return value
 *              of the last executed program, and envrionmental variables
 *              preceded by $ with their corresponding value.
 */
void vari_rep(char **line, int *exe_ret)
{
	int l, m = 0, len;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (l = 0; old_line[l]; l++)
	{
		if (old_line[l] == '$' && old_line[l + 1] &&
				old_line[l + 1] != ' ')
		{
			if (old_line[l + 1] == '$')
			{
				replacement = get_pid();
				m = l + 2;
			}
			else if (old_line[l + 1] == '?')
			{
				replacement = _ita(*exe_ret);
				m = l + 2;
			}
			else if (old_line[l + 1])
			{
				/* extract the variable name to search for */
				for (m = l + 1; old_line[m] &&
						old_line[m] != '$' &&
						old_line[m] != ' '; m++)
					;
				len = m - (l + 1);
				replacement = get_env_value(&old_line[l + 1], len);
			}
			new_line = malloc(l + _strlen(replacement)
					  + _strlen(&old_line[m]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, l);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[m]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			l = -1;
		}
	}
}
