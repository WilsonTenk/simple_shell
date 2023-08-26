#include "main.h"
#include <stdio.h>

/**
 * willy_cmd_exit - Exits the program
 * @sh: Pointer to the shell structure
 */
static void willy_cmd_exit(shell *sh)
{
	if (sh->args[1])
	{
		if (!willy_is_num(sh->args[1]) || sh->args[1][0] == '-')
		{
			willy_fprintf(STDERR_FILENO, "%s: 1: exit: Illegal number: %s\n",
					 "./hsh", sh->args[1]);
			sh->status = 2;
			return;
		}

		if (sh->args[2])
		{
			willy_fprintf(STDERR_FILENO, "exit: too many arguments\n");
			sh->status = 1;
			return;
		}

		if (willy_is_num(sh->args[1]))
			sh->status = willy_atoi(sh->args[1]);
	}

	sh->run = 0;
}

/**
 * willy_cmd_env - Prints the current environment
 * @sh: Pointer to the shell structure
 */
static void willy_cmd_env(shell *sh)
{
	unsigned int i;
	(void)(sh);

	if (!environ)
	{
		willy_fprintf(STDERR_FILENO, "env: environ is NULL\n");
		return;
	}
	for (i = 0; environ[i]; i++)
		willy_printf("%s\n", environ[i]);

	sh->status = 0;
}

/**
 * willy_cmd_cd - Change the current working directory
 * @sh: Pointer to the shell structure
 */
static void willy_cmd_cd(shell *sh)
{
	int chdir_status;
	char buf[BUFFER_SIZE];
	char *new_dir, *old_dir, *oldpwd_var, *pwd_var;

	old_dir = willy_getcwd(buf, BUFFER_SIZE);

	if (!sh->args[1])
		new_dir = willy_getenv("HOME");
	else if (willy_strcmp(sh->args[1], "-", -1) == 0)
	{
		new_dir = willy_getenv("OLDPWD");
		if (new_dir)
			willy_printf("%s\n", new_dir);
		else
		{
			new_dir = old_dir;
			willy_printf("%s\n", old_dir);
		}
	}
	else
		new_dir = sh->args[1];

	if (!new_dir)
		return;
	/* Change directory */
	chdir_status = willy_chdir(new_dir);
	if (chdir_status == 0)
	{
		/* Set OLDPWD to current working directory */
		oldpwd_var = willy_malloc(100);
		willy_sprintf(oldpwd_var, "OLDPWD=%s", old_dir);
		willy_update_environment(sh, oldpwd_var);
		/* Allocate memory and create new env variables and update it */
		pwd_var = willy_malloc(100);
		willy_sprintf(pwd_var, "PWD=%s", willy_getcwd(buf, BUFFER_SIZE));
		willy_update_environment(sh, pwd_var);
	}
	else if (sh->args[1])
		willy_fprintf(STDERR_FILENO, "%s: 1: cd: can't cd to %s\n",
				 "./hsh", sh->args[1]);
}

/**
 * willy_get_builtins - Returns an array of builtin commands
 * Return: Pointer to the array of builtin commands
 */
cmd *willy_get_builtins(void)
{
	static cmd builtins[] = {
		{"exit", willy_cmd_exit},
		{"env", willy_cmd_env},
		{"cd", willy_cmd_cd},
		{"setenv", willy_cmd_setenv},
		{"unsetenv", willy_cmd_unsetenv},
		{"alias", willy_cmd_alias},
		{NULL, NULL},
	};
	return (builtins);
}
