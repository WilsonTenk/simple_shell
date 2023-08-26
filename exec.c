#include "main.h"
#include <stdio.h>

/**
 * willy_builtin_command - Executes a built-in cmd if it is found
 * @sh: Pointer to the shell structure
 * Return: Index of the built-in cmd that was executed, or 0 if none was found
 */
int willy_builtin_command(shell *sh)
{
	int i;

	for (i = 0; i < sh->num_builtins; i++)
	{
		if (sh->args[0] && willy_strcmp(sh->args[0], sh->builtins[i].name, -1) == 0)
		{
			sh->builtins[i].func(sh);
			break;
		}
	}
	return (i);
}

/**
 * willy_external_command - Executes an external command
 * @sh: Pointer to the shell structure
 * @curr_line: Pointer to the current line number
 */
void willy_external_command(shell *sh, int *curr_line)
{
	pid_t pid;
	int ret, wstatus;
	char *full_path = NULL;

	/* Check if the command is an absolute or relative path */
	if (sh->args[0][0] == '/' || sh->args[0][0] == '.')
		full_path = sh->args[0];
	/* OR find the full path of the command */
	else
		full_path = willy_find_command(sh->args[0]);

	if (full_path)
	{
		pid = willy_fork();
		if (pid == 0)
		{
			ret = willy_execve(full_path, sh->args, environ);
			if (ret == -1)
				willy_perror(sh->args[0]);
			willy_exit(ret);
		}
		else
		{
			willy_wait(&wstatus);
			/* Get the exit status of the child */
			if (willy_WIFEXITED(wstatus))
				sh->status = willy_WEXITSTATUS(wstatus);
		}
		if (full_path != sh->args[0])
			willy_free(full_path);
	}
	else
	{
		if (sh->interactive)
			willy_fprintf(STDERR_FILENO, "%s: command not found\n", sh->args[0]);
		else
			willy_fprintf(STDERR_FILENO, "%s: %d: %s: not found\n",
					 "./hsh", *curr_line, sh->args[0]);
		sh->status = 127;
	}
}

/**
 * willy_execute_command - Executes a command
 * @sh: Pointer to the shell structure
 * @curr_line: Pointer to the current line number
 */
void willy_execute_command(shell *sh, int *curr_line)
{
	int j;
	char *alias_value;

	j = willy_builtin_command(sh);
	if (j == sh->num_builtins)
	{
		/* TODO: add aliases contain also args */
		alias_value = willy_get_alias_value(sh, sh->args[0]);
		if (alias_value)
			sh->args[0] = alias_value;

		willy_external_command(sh, curr_line);
	}
	(*curr_line)++;

	willy_free(sh->args);
	sh->args = NULL;
}

/**
 * willy_process_command - Reads input, parses it, and executes the cmd
 * @sh: Pointer to the shell structure
 * NOTE: '&&' still doesn't handle failed cmd
 */
void willy_process_command(shell *sh)
{
	int i, curr_line = 1;
	char *oprs = ";|&", *saveptr, *cmd;

	willy_read_input(sh);
	if (!sh->input)
		return;

	for (i = 0; i < sh->cmd_count; i++)
	{
		cmd = willy_strtok_r(sh->input[i], oprs, &saveptr);
		while (cmd != NULL)
		{
			/* Parse the command and its arguments */
			willy_parse_command(sh, cmd);
			/* Check if sh->args[0] is NULL or an empty string */
			if (sh->args[0] && sh->args[0][0])
				willy_execute_command(sh, &curr_line);

			/* Check for || and previous cmd success */
			if (saveptr[0] == '|' && sh->status == 0)
				break;
			/* Check for && and previous cmd failed */
			if (saveptr[0] == '&' && sh->status != 0)
				break;

			cmd = willy_strtok_r(NULL, oprs, &saveptr);
		}
	}

	sh->cmd_count = 0;

	if (sh->input)
		willy_free_double(&sh->input);
}
