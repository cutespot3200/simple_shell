#include "shell.h"

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b);
ssize_t get_line(char **lineptr, size_t *n, FILE *stream);

/**
 * _realloc - Reassign memory block using malloc and free.
 * @ptr: Pointer to memory previously allocated.
 * @old_size: Size in bytes of the allocated space for ptr.
 * @new_size: Size in bytes for the new memory block.
 *
 * Return: Where new_size == old_size - ptr.
 *         Where new_size == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mim;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mim = malloc(new_size);
		if (mim == NULL)
			return (NULL);

		return (mim);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mim = malloc(sizeof(*ptr_copy) * new_size);
	if (mim == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mim;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mim);
}

/**
 * assign_lineptr - Reassign lineptr var for get_line.
 * @lineptr: Buffer to store input str.
 * @n: Size of lineptr.
 * @buffer: Str to assign to lineptr.
 * @b: Size of buffer.
 */
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b)
{
	if (*lineptr == NULL)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * getline - skim input from a stream.
 * @lineptr: Buffer to store the input.
 * @n: Size of lineptr.
 * @stream: Stream to read from.
 *
 * Return: Number of bytes read.
 */
ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	static ssize_t input;
	ssize_t rot;
	char c = 'x', *buffer;
	int r;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	assign_lineptr(lineptr, n, buffer, input);

	rot = input;
	if (r != 0)
		input = 0;
	return (rot);
}
