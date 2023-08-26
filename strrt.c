#include "main.h"

/**
 * _strlen - String lengths
 * @s: Given string
 * Return: The given length of a string
 */
int _strlen(char *s)
{
	int len = 0;

	while (s[len])
		len++;

	return (len);
}

/**
 * reverse_str - This reverses a given string.
 * @s: A string to reverse
 */
void reverse_str(char s[])
{
	char temp;
	unsigned int t = 0, len = 0;

	len = _strlen(s);

	for (; t < len / 2; t++)
	{
		temp = s[t];
		s[t] = s[len - t - 1];
		s[len - t - 1] = temp;
	}
}

/**
 * _stoa - It copy a string to a buffer
 * @s: String to be copied
 * @buf: Buffer to copy it to
 * Return: Length of the string copied.
 */
int _stoa(char *s, char *buf)
{
	int t = 0;
	char *p = (!s) ? "(null)" : s;

	while (*p)
		buf[t++] = *p++;
	return (t);
}

/**
 * _strcmp - Two strings to be compared
 * @s1: 1st string to compare.
 * @s2: 2nd string to compare.
 * @n: char indexes to compare
 * Return: What differentiate the two strings
 */
int _strcmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);

	while (n && *s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
		n--;
	}

	if (n == 0)
		return (0);

	return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

/**
 * _memcpy - This copies mem from one place to another
 * @dest: Pointer to final memory
 * @src: Pointer to source memory
 * @n: Number of bytes to be copied
 * Return: Pointer to the final memory
 */
void *_memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	while (n--)
		*d++ = *s++;
	return (dest);
}
