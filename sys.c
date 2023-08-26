#include "main.h"
#include <stdio.h>

#define LINE_SIZE 1024

/**
 * _check_alloc - Ensure allocated memory is sufficient for a given size
 * @line: Pointer to allocated memory
 * @size: Size of data to be stored
 * @alloc_size: Pointer to allocated memory's size
 * Return: Pointer to allocated memory
 */
char *_check_alloc(char *line, ssize_t size, ssize_t *alloc_size)
{
	if (*alloc_size == 0)
	{
		*alloc_size = BUFFER_SIZE;
		line = _custom_realloc(line, size, *alloc_size);
	}
	else if (size + 2 > *alloc_size)
	{
		*alloc_size *= 2;
		line = _custom_realloc(line, size, *alloc_size);
	}
	return (line);
}

/**
 * _willy_getline - Read a line from a file descriptor (EOF)
 * @lineptr: Pointer to the buffer for the line
 * @n: Size of the buffer
 * @fd: File descriptor
 * Return: Number of bytes read, or -1 on failure
 */
ssize_t _willy_getline(char **lineptr, size_t *n, int fd)
{
	static char buffer[BUFFER_SIZE];
	static size_t start, end;
	char *line = *lineptr;
	ssize_t bytes = 0, size = 0, byte = 0, alloc_size = *n;

	for (;;)
	{
		if (start >= end)
		{
			bytes = willy_read(fd, buffer, BUFFER_SIZE);
			if (bytes <= 0 && size > 0 && line[size - 1] != '\n')
			{
				line = _check_alloc(line, size, &alloc_size);
				if (!line)
					return (-1);
				line[size++] = '\n';
			}
			else if (bytes <= 0)
				return (-1);
			start = 0;
			end = bytes;
		}
		while (start < end)
		{
			byte = buffer[start++];
			line = _check_alloc(line, size, &alloc_size);
			if (!line)
				return (-1);
			line[size++] = byte;
			if (byte == '\n')
				break;
		}
		if (byte == '\n' || bytes <= 0)
		{
			line[size] = '\0';
			*lineptr = line;
			*n = size;
			return (size);
		}
	}
}

/**
 * _willy_getenv - Get the value of an environment variable
 * @name: Name of the environment variable
 * Return: Pointer to the value, or NULL if not found
 */
char *_willy_getenv(const char *name)
{
	size_t len, i;

	if (!name || !environ)
		return (NULL);

	len = _willy_strlen((char *)name);

	for (i = 0; environ[i]; i++)
		/* Check if string starts with the name of the env variable */
		if (_willy_strcmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (&environ[i][len + 1]);

	return (NULL);
}

/**
 * willy_copy_environ - Copy environment variables
 * Return: Pointer to the new array, or NULL if fails
 */
char **willy_copy_environ(void)
{
	size_t env_count = 0;
	size_t new_size;
	char **new_environ, **env_ptr;

	/* Count the number of environment variables */
	for (env_ptr = environ; *env_ptr; env_ptr++)
		env_count++;

	new_size = (env_count + 1) * sizeof(char *);
	new_environ = willy_malloc(new_size);

	if (!new_environ)
	{
		willy_fprintf(STDERR_FILENO, "Failed to allocate memory\n");
		return (NULL);
	}

	/* Copy the environment variables */
	for (env_count = 0; environ[env_count]; env_count++)
	{
		new_environ[env_count] = _willy_strdup(environ[env_count]);
		if (!new_environ[env_count])
		{
			willy_fprintf(STDERR_FILENO, "Failed to allocate memory\n");
			return (NULL);
		}
	}

	/* Set the last element of the array to NULL */
	new_environ[env_count] = NULL;

	return (new_environ);
}
