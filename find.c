#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

/**
 * willy_find_command - Finds the full path of a command in the PATH environment
 * @command: The command to search for
 * Return: A pointer to the full path of the command, or NULL if not found
 */
char *willy_find_command(char *command)
{
	char *path = NULL, *path_copy = NULL;
	char *dir = NULL, *full_path = NULL;

	path = willy_getenv("PATH");
	if (!path)
		return (NULL);

	path_copy = willy_strdup(path);
	dir = willy_strtok(path_copy, ":");
	full_path = willy_malloc(willy_strlen(command) + willy_strlen(path) + 2);

	while (dir != NULL)
	{
		willy_sprintf(full_path, "%s/%s", dir, command);
		if (willy_access(full_path, X_OK) == 0)
		{
			willy_free(path_copy);
			return (full_path);
		}
		dir = willy_strtok(NULL, ":");
	}

	willy_free(path_copy);
	willy_free(full_path);
	return (NULL);
}
