#include "main.h"

/**
 * willy_find_environment - Finds the index of an env variable in the environ
 * @name: The name of the environment variable to search for
 * Return: The index of the environment variable, or -1 if not found.
 */
int willy_find_environment(char *name)
{
	char **env;
	int index = 0;
	size_t name_len = willy_strlen(name);

	for (env = environ; *env != NULL; env++, index++)
		/* Compare the environment variable name with the given name */
		if (willy_strcmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
			return (index);

	return (-1);
}

/**
 * willy_update_environment - Updates the environment variables
 * @sh: Pointer to the shell structure
 * @env_var: The new environment variable to add
 */
void willy_update_environment(shell *sh, char *env_var)
{
	size_t env_count = 0, name_len;
	size_t old_size, new_size;
	char **new_environ, **env_ptr;
	int index;
	char *name_end = willy_strchr(env_var, '=');
	char name[BUFFER_SIZE];

	if (!name_end)
		return;
	if (!sh->environ_copy)
		sh->environ_copy = willy_copy_environ();

	name_len = name_end - env_var;
	willy_memcpy(name, env_var, name_len);
	name[name_len] = '\0';
	index = willy_find_environment(name);
	if (index >= 0)
	{
		willy_free(sh->environ_copy[index]);
		sh->environ_copy[index] = env_var;
	}
	else
	{
		for (env_ptr = sh->environ_copy; *env_ptr; env_ptr++)
			env_count++;
		old_size = env_count * sizeof(char *);
		new_size = (env_count + 2) * sizeof(char *);
		new_environ = willy_realloc(sh->environ_copy, old_size, new_size);
		if (!new_environ)
		{
			willy_fprintf(STDERR_FILENO, "Failed to allocate memory\n");
			willy_free(env_var);
			return;
		}
		sh->environ_copy = new_environ;
		sh->environ_copy[env_count] = env_var;
		sh->environ_copy[env_count + 1] = NULL;
	}
	environ = sh->environ_copy;
	sh->status = 0;
}

/**
 * willy_remove_environment - Removes an environment variable from the system
 * @sh: Pointer to the shell structure
 */
void willy_remove_environment(shell *sh)
{
	int index = willy_find_environment(sh->args[1]);

	/* If the environment variable is not found, return */
	if (index == -1)
		return;

	if (!sh->environ_copy)
		sh->environ_copy = willy_copy_environ();

	willy_free(sh->environ_copy[index]);

	/* Shift the remaining environment variables up by one */
	for (; sh->environ_copy[index] != NULL; index++)
		sh->environ_copy[index] = sh->environ_copy[index + 1];

	/* Update the global environ variable */
	environ = sh->environ_copy;
	sh->status = 0;
}

/**
 * willy_cmd_unsetenv - Unsets an environment variable
 * @sh: Pointer to the shell structure
 */
void willy_cmd_unsetenv(shell *sh)
{
	if (sh->args[1] == NULL)
	{
		willy_fprintf(STDERR_FILENO, "Usage: unsetenv VARIABLE\n");
		sh->status = 2;
		return;
	}

	willy_remove_environment(sh);
}

/**
 * willy_cmd_setenv - Sets an environment variable
 * @sh: Pointer to the shell structure
 */
void willy_cmd_setenv(shell *sh)
{
	char *env_var;
	size_t name_len, value_len, env_var_len;

	if (!sh->args[1] || !sh->args[2])
	{
		willy_fprintf(STDERR_FILENO, "Usage: setenv VARIABLE VALUE\n");
		sh->status = 2;
		return;
	}

	name_len = willy_strlen(sh->args[1]);
	value_len = willy_strlen(sh->args[2]);

	/* +2 for '=' and '\0' */
	env_var_len = name_len + value_len + 2;
	env_var = willy_malloc(env_var_len);

	if (!env_var)
	{
		willy_fprintf(STDERR_FILENO, "Failed to allocate memory\n");
		sh->status = 1;
		return;
	}
	willy_sprintf(env_var, "%s=%s", sh->args[1], sh->args[2]);

	willy_update_environment(sh, env_var);
}
