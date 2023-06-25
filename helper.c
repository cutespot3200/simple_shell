#include "shell.h"

void hold_line(char **line, ssize_t read);
ssize_t get_new_len(char *line);
void logical_ops(char *line, ssize_t *new_len);

/**
 * hold_line - Divides a line read from stdin .
 * @line: Pointer to a line read from stdin.
 * @read: Length of line.
 *
 * Description: Spaces are inserted to separate ";", "||", and "&&".
 *              Replaces "#" with '\0'.
 */
void hold_line(char **line, ssize_t read)
{
	char *old_line, *new_line;
	char previous, current, next;
	size_t l, m;
	ssize_t new_len;

	new_len = get_new_len(*line);
	if (new_len == read - 1)
		return;
	new_line = malloc(new_len + 1);
	if (!new_line)
		return;
	m = 0;
	old_line = *line;
	for (l = 0; old_line[l]; l++)
	{
		current = old_line[l];
		next = old_line[l + 1];
		if (l != 0)
		{
			previous = old_line[l - 1];
			if (current == ';')
			{
				if (next == ';' && previous != ' ' && previous != ';')
				{
					new_line[m++] = ' ';
					new_line[m++] = ';';
					continue;
				}
				else if (previous == ';' && next != ' ')
				{
					new_line[m++] = ';';
					new_line[m++] = ' ';
					continue;
				}
				if (previous != ' ')
					new_line[m++] = ' ';
				new_line[m++] = ';';
				if (next != ' ')
					new_line[m++] = ' ';
				continue;
			}
			else if (current == '&')
			{
				if (next == '&' && previous != ' ')
					new_line[m++] = ' ';
				else if (previous == '&' && next != ' ')
				{
					new_line[m++] = '&';
					new_line[m++] = ' ';
					continue;
				}
			}
			else if (current == '|')
			{
				if (next == '|' && previous != ' ')
					new_line[m++]  = ' ';
				else if (previous == '|' && next != ' ')
				{
					new_line[m++] = '|';
					new_line[m++] = ' ';
					continue;
				}
			}
		}
		else if (current == ';')
		{
			if (l != 0 && old_line[l - 1] != ' ')
				new_line[m++] = ' ';
			new_line[m++] = ';';
			if (next != ' ' && next != ';')
				new_line[m++] = ' ';
			continue;
		}
		new_line[m++] = old_line[l];
	}
	new_line[m] = '\0';

	free(*line);
	*line = new_line;
}

/**
 * get_new_len - Takes new len of a line partition
 *               by ";", "||", "&&&", or "#".
 * @line: Line to check.
 *
 * Return: Length of the line.
 *
 * Description: Cut short lines containing '#' comments with '\0'.
 */

ssize_t get_new_len(char *line)
{
	size_t l;
	ssize_t new_len = 0;
	char current, next;

	for (l = 0; line[l]; l++)
	{
		current = line[l];
		next = line[l + 1];
		if (current == '#')
		{
			if (l == 0 || line[l - 1] == ' ')
			{
				line[l] = '\0';
				break;
			}
		}
		else if (l != 0)
		{
			if (current == ';')
			{
				if (next == ';' && line[l - 1] != ' ' && line[l - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (line[l - 1] == ';' && next != ' ')
				{
					new_len += 2;
					continue;
				}
				if (line[l - 1] != ' ')
					new_len++;
				if (next != ' ')
					new_len++;
			}
			else
				logical_ops(&line[l], &new_len);
		}
		else if (current == ';')
		{
			if (l != 0 && line[l - 1] != ' ')
				new_len++;
			if (next != ' ' && next != ';')
				new_len++;
		}
		new_len++;
	}
	return (new_len);
}
/**
 * logical_ops - Detect line for logical operators "||" or "&&".
 * @line: Pointer to the char to check in the line.
 * @new_len: Pointer to new_len in get_new_len function.
 */
void logical_ops(char *line, ssize_t *new_len)
{
	char previous, current, next;

	previous = *(line - 1);
	current = *line;
	next = *(line + 1);

	if (current == '&')
	{
		if (next == '&' && previous != ' ')
			(*new_len)++;
		else if (previous == '&' && next != ' ')
			(*new_len)++;
	}
	else if (current == '|')
	{
		if (next == '|' && previous != ' ')
			(*new_len)++;
		else if (previous == '|' && next != ' ')
			(*new_len)++;
	}
}
