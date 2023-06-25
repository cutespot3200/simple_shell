#include "shell.h"

char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);

/**
 * _strchr - Loc char of a str.
 * @s: String to be searched.
 * @c: Char to be located.
 *
 * Return: Where c is found - a pointer to the first occurence.
 *         Where c is not found - NULL.
 */
char *_strchr(char *s, char c)
{
	int ind;

	for (ind = 0; s[ind]; ind++)
	{
		if (s[ind] == c)
			return (s + ind);
	}

	return (NULL);
}

/**
 * _strspn - Takes the len of a prefix substring.
 * @s: String to be searched.
 * @accept: Prefix to be measured.
 *
 * Return: Number of bytes in s which
 *         consist only of bytes from accept.
 */
int _strspn(char *s, char *accept)
{
	int bytes = 0;
	int ind;

	while (*s)
	{
		for (ind = 0; accept[ind]; ind++)
		{
			if (*s == accept[ind])
			{
				bytes++;
				break;
			}
		}
		s++;
	}
	return (bytes);
}

/**
 * _strcmp - Compares two str.
 * @s1: First str to be compared.
 * @s2: Second str to be compared.
 *
 * Return: Positive byte diff if s1 > s2
 *         0 if s1 = s2
 *         Negative byte diff if s1 < s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (*s1 != *s2)
		return (*s1 - *s2);

	return (0);
}

/**
 * _strncmp - Compare 2 str.
 * @s1: A pointer to str.
 * @s2: A Pointer to str.
 * @n: First n bytes of the strings to compare.
 *
 * Return: Less than 0 if s1 is shorter than s2.
 *         0 if s1 and s2 match.
 *         Greater than 0 if s1 is longer than s2.
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t l;

	for (l = 0; s1[l] && s2[l] && l < n; l++)
	{
		if (s1[l] > s2[l])
			return (s1[l] - s2[l]);
		else if (s1[l] < s2[l])
			return (s1[l] - s2[l]);
	}
	if (l == n)
		return (0);
	else
		return (-15);
}
