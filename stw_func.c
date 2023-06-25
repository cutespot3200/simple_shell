#include "shell.h"

int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);

/**
 * _strlen - Return len of str.
 * @s: Pointer to char str.
 *
 * Return: Length of char str.
 */
int _strlen(const char *s)
{
	int span = 0;

	if (!s)
		return (span);
	for (span = 0; s[span]; span++)
		;
	return (span);
}

/**
 * _strcpy - Duplicate str  pointed to by src, including the
 *           terminating null byte, to the buffer pointed by des.
 * @dest: A pointer to the destination of duplicated str.
 * @src: A pointer to the src of the source str.
 *
 * Return: A pointer to dest.
 */
char *_strcpy(char *dest, const char *src)
{
	size_t m;

	for (m = 0; src[m] != '\0'; m++)
		dest[m] = src[m];
	dest[m] = '\0';
	return (dest);
}

/**
 * _strcat - Concat 2 str.
 * @dest: A pointer to destination str.
 * @src: A Pointer to source str.
 *
 * Return: A pointer to destination str.
 */
char *_strcat(char *dest, const char *src)
{
	char *dest_str;
	const char *src_Temp;

	dest_str = dest;
	src_Temp =  src;

	while (*dest_str != '\0')
		dest_str++;

	while (*src_Temp != '\0')
		*dest_str++ = *src_Temp++;
	*dest_str = '\0';
	return (dest);
}

/**
 * _strncat - Concat 2 str if n number
 *            of bytes are copied from source.
 * @dest: A pointer to destination str.
 * @src: A pointer to source str.
 * @n: n bytes to copy from src.
 *
 * Return: A pointer to destination str.
 */
char *_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = _strlen(dest);
	size_t m;

	for (m = 0; m < n && src[m] != '\0'; m++)
		dest[dest_len + m] = src[m];
	dest[dest_len + m] = '\0';

	return (dest);
}
