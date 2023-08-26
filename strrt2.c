#include "main.h"

/**
 * write_str - Writes a string to a file descriptor
 * @fd: The file descriptor to write to
 * @s: The string to write
 */
void write_str(int fd, char *s)
{
	(!s) && (s = "(null)");
	write(fd, s, _willy_strlen(s));
}

/**
 * _willy_strchr - searches for a character in a string
 * @s: pointer to the string to search
 * @c: character to search for
 * Return: Pointer to the first occurrence of c in s, or NULL if not found
 */
char *_willy_strchr(char *s, int c)
{
	while (*s != (char)c)
		if (!*s++)
			return (0);

	return (s);
}

/**
 * _willy_strdup - Duplicates a string
 * @s: The string to duplicate
 * Return: Pointer to the duplicated string, or NULL if fails
 */
char *_willy_strdup(const char *s)
{
	int i;
	char *result;

	result = willy_malloc(sizeof(char) * (_willy_strlen((char *)s) + 1));
	if (result == NULL)
		return (NULL);

	for (i = 0; s[i]; i++)
		result[i] = s[i];

	result[i] = '\0';

	return (result);
}

/**
 * _willy_strtok - Tokenizes a string by a given delimiter
 * @str: the string to split
 * @delim: The delimiter used for tokenization
 * Return: A pointer to the next token in the string
 */
char *_willy_strtok(char *str, char *delim)
{
	static char *next;
	char *curr = str ? str : next;

	if (!curr)
		return (NULL);

	while (*curr && _willy_strchr(delim, *curr))
		curr++;
	if (!*curr)
		return (NULL);

	next = curr;
	while (*next)
	{
		if (*next == '"')
		{
			next++;
			while (*next && *next != '"')
				next++;
			if (*next == '"')
				next++;
		}
		else if (_willy_strchr(delim, *next))
		{
			break;
		}
		else
			next++;
	}
	if (*next)
		*next++ = '\0';
	return (curr);
}

/**
 * _willy_strtok_r - tokenizes a string using a delimiter (safe)
 * @str: The string to split
 * @delim: The delimiter character used to split the string
 * @saveptr: pointer to save the position of the next token
 *
 * used instead of _willy_strtok cos is reentrant n can be safely used in
 * multi-process. The _willy_strtok function uses a static variable
 * the position in the string between calls, which can cause issues if the
 * function is called from multiple threads or if it is called recursively.
 *
 * Return: pointer to the start of the token, or NULL if no more tokens
 */
char *_willy_strtok_r(char *str, const char *delim, char **saveptr)
{
	char *token_start, *token_end;
	const char *d;

	if (str == NULL)
		str = *saveptr;
	token_start = str;
	while (*token_start != '\0')
	{
		for (d = delim; *d != '\0'; d++)
			if (*token_start == *d)
				break;
		if (*d == '\0')
			break;
		token_start++;
	}
	if (*token_start == '\0')
	{
		*saveptr = token_start;
		return (NULL);
	}
	token_end = token_start;
	while (*token_end != '\0')
	{
		for (d = delim; *d != '\0'; d++)
			if (*token_end == *d)
				break;
		if (*d != '\0')
			break;
		token_end++;
	}
	if (*token_end != '\0')
	{
		*token_end = '\0';
		*saveptr = token_end + 1;
	}
	else
		*saveptr = token_end;

	return (token_start);
}
