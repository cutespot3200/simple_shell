#include "shell.h"

char *full_path_dir(char *path);
list_t *go_paths_dir(char *path);

/**
 * go_loc - Find a command in the PATH.
 * @command: Command to locate.
 *
 * Return: Where error occur or the command cannot be located - NULL.
 *         Otherwise - the full pathname of the command.
 */
char *go_loc(char *command)
{
	char **path, *tem;
	list_t *dirs, *head;
	struct stat stu;

	path = _goenv("PATH");
	if (!path || !(*path))
		return (NULL);

	dirs = go_paths_dir(*path + 5);
	head = dirs;

	while (dirs)
	{
		tem = malloc(_strlen(dirs->dir) + _strlen(command) + 2);
		if (!tem)
			return (NULL);

		_strcpy(tem, dirs->dir);
		_strcat(tem, "/");
		_strcat(tem, command);

		if (stat(tem, &stu) == 0)
		{
			free_list(head);
			return (tem);
		}

		dirs = dirs->next;
		free(tem);
	}

	free_list(head);

	return (NULL);
}

/**
 * full_path_dir - Duplicate path n also replaces leading/sandwiched/trailing
 *		   colons (:) with current working directory.
 * @path: Colon-separated list of directories.
 *
 * Return: Copy of path with any leading/sandwiched/trailing colons replaced
 *	   with the current working directory.
 */
char *full_path_dir(char *path)
{
	int l, length = 0;
	char *path_copy, *pwd;

	pwd = *(_goenv("PWD")) + 4;
	for (l = 0; path[l]; l++)
	{
		if (path[l] == ':')
		{
			if (path[l + 1] == ':' || l == 0 || path[l + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	path_copy = malloc(sizeof(char) * (length + 1));
	if (!path_copy)
		return (NULL);
	path_copy[0] = '\0';
	for (l = 0; path[l]; l++)
	{
		if (path[l] == ':')
		{
			if (l == 0)
			{
				_strcat(path_copy, pwd);
				_strcat(path_copy, ":");
			}
			else if (path[l + 1] == ':' || path[l + 1] == '\0')
			{
				_strcat(path_copy, ":");
				_strcat(path_copy, pwd);
			}
			else
				_strcat(path_copy, ":");
		}
		else
		{
			_strncat(path_copy, &path[l], 1);
		}
	}
	return (path_copy);
}

/**
 * go_paths_dir - Tokenizes a colon-separated list of
 *                directories into a list_s linked list.
 * @path: Colon-separated list of directories.
 *
 * Return: Pointer to initialized linked list.
 */
list_t *go_paths_dir(char *path)
{
	int ind;
	char **dirs, *path_copy;
	list_t *head = NULL;

	path_copy = full_path_dir(path);
	if (!path_copy)
		return (NULL);
	dirs = _strtok(path_copy, ":");
	free(path_copy);
	if (!dirs)
		return (NULL);

	for (ind = 0; dirs[ind]; ind++)
	{
		if (add_apex(&head, dirs[ind]) == NULL)
		{
			free_list(head);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (head);
}
