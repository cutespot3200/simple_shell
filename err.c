#include "shell.h"

int num_len(int num);
char *_ita(int num);
int make_error(char **args, int err);

/**
 * num_len - Counts the len of number.
 * @num: Number to measure.
 *
 * Return: Digit length.
 */
int num_len(int num)
{
	unsigned int num2;
	int len = 1;

	if (num < 0)
	{
		len++;
		num2 = num * -1;
	}
	else
	{
		num2 = num;
	}
	while (num2 > 9)
	{
		len++;
		num2 /= 10;
	}

	return (len);
}

/**
 * _ita - Change an int to a str.
 * @num: Integer.
 *
 * Return: Converted string.
 */
char *_ita(int num)
{
	char *buffer;
	int len = num_len(num);
	unsigned int num2;

	buffer = malloc(sizeof(char) * (len + 1));
	if (!buffer)
		return (NULL);

	buffer[len] = '\0';

	if (num < 0)
	{
		num2 = num * -1;
		buffer[0] = '-';
	}
	else
	{
		num2 = num;
	}

	len--;
	do {
		buffer[len] = (num2 % 10) + '0';
		num2 /= 10;
		len--;
	} while (num2 > 0);

	return (buffer);
}


/**
 * make_error - Writes a custom error message to stderr.
 * @args: Array of args.
 * @err: Error value.
 *
 * Return: Error value.
 */
int make_error(char **args, int err)
{
	char *error;

	switch (err)
	{
	case -1:
		error = err_env(args);
		break;
	case 1:
		error = err_1(args);
		break;
	case 2:
		if (*(args[0]) == 'e')
			error = err_2_exit(++args);
		else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
			error = err_2_syntax(args);
		else
			error = err_2_cd(args);
		break;
	case 126:
		error = err_126(args);
		break;
	case 127:
		error = err_127(args);
		break;
	}
	write(STDERR_FILENO, error, _strlen(error));

	if (error)
		free(error);
	return (err);

}
