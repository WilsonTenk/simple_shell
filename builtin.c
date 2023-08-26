#include "main.h"
#include <stdio.h>

/**
 * cmd_exit - Shoud exits our program
 * @wt: Shell struct pointed at
 */
static void cmd_exit(shell *wt)
{
	if (wt->args[1])
	{
		if (!num_val(wt->args[1]) || wt->args[1][0] == '-')
		{
			_fprintf(STDERR_FILENO, "%s: 1: exit: Illegal numb: %s\n",
					 "./hsh", wt->args[1]);
			wt->status = 2;
			return;
		}

		if (wt->args[2])
		{
			_fprintf(STDERR_FILENO, "exit: too many args\n");
			wt->status = 1;
			return;
		}

		if (num_val(wt->args[1]))
			wt->status = _atoi(wt->args[1]);
	}

	wt->run = 0;
}

/**
 * cmd_env - Should output the current environ
 * @wt: Struct to be pointed at
 */
static void cmd_env(shell *wt)
{
	unsigned int u;
	(void)(wt);

	if (!environ)
	{
		_fprintf(STDERR_FILENO, "env: env is NULL\n");
		return;
	}
	for (u = 0; environ[u]; u++)
		_printf("%s\n", environ[u]);

	wt->status = 0;
}

/**
 * cmd_cd - The cd the directory we are in
 * @wt: Shell struct to be pointed at.
 */
static void cmd_cd(shell *wt)
{
	int chdir_status;
	char buf[BUFFER_SIZE];
	char *new_dir, *old_dir, *oldpwd_var, *pwd_var;

	old_dir = getcwd(buf, BUFFER_SIZE);

	if (!wt->args[1])
		new_dir = _getenv("HOME");
	else if (_strcmp(wt->args[1], "-", -1) == 0)
	{
		new_dir = _getenv("OLDPWD");
		if (new_dir)
			_printf("%s\n", new_dir);
		else
		{
			new_dir = old_dir;
			_printf("%s\n", old_dir);
		}
	}
	else
		new_dir = wt->args[1];

	if (!new_dir)
		return;
	/* Should Change directory */
	chdir_status = chdir(new_dir);
	if (chdir_status == 0)
	{
		/* Setting an OLDPWD to current work directory */
		oldpwd_var = malloc(100);
		_sprintf(oldpwd_var, "OLDPWD=%s", old_dir);
		update_environment(wt, oldpwd_var);
		/* Create new env variables, allocate new mem, and upadte it */
		pwd_var = malloc(100);
		_sprintf(pwd_var, "PWD=%s", getcwd(buf, BUFFER_SIZE));
		update_environment(wt, pwd_var);
	}
	else if (wt->args[1])
		_fprintf(STDERR_FILENO, "%s: 1: cd: can't cd to %s\n",
				 "./hsh", wt->args[1]);
}

/**
 * get_builtins - An array of builtin commands is returned
 * Return: There is a Pointer to the array of builtin commands
 */
cmd *get_builtins(void)
{
	static cmd builtins[] = {
		{"exit", cmd_exit},
		{"env", cmd_env},
		{"cd", cmd_cd},
		{"setenv", cmd_setenv},
		{"unsetenv", cmd_unsetenv},
		{"alias", cmd_alias},
		{NULL, NULL},
	};
	return (builtins);
}
