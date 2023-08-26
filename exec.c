#include "main.h"
#include <stdio.h>

/**
 * builtin_command - Carry out a builtin cmd if found
 * @sh: Shell structure's pointer
 * Return: Builtin index carried out, or 0 none found
 */
int builtin_command(shell *sh)
{
	int u;

	for (u = 0; u < sh->num_builtins; u++)
	{
		if (sh->args[0] && _strcmp(sh->args[0], sh->builtins[u].name, -1) == 0)
		{
			sh->builtins[u].func(sh);
			break;
		}
	}
	return (u);
}

/**
 * external_command - we have our external cmd
 * @sh: Shell structure's pointer
 * @curr_line: Current line numb's pointer
 */
void external_command(shell *sh, int *curr_line)
{
	pid_t pid;
	int ret, wstatus;
	char *full_path = NULL;

	/* Search in case the command is an absolute or relative path */
	if (sh->args[0][0] == '/' || sh->args[0][0] == '.')
		full_path = sh->args[0];
	/* else check full path of the cmd */
	else
		full_path = find_command(sh->args[0]);

	if (full_path)
	{
		pid = fork();
		if (pid == 0)
		{
			ret = execve(full_path, sh->args, environ);
			if (ret == -1)
				perror(sh->args[0]);
			_exit(ret);
		}
		else
		{
			wait(&wstatus);
			/* Must get the exit status of our child */
			if (WIFEXITED(wstatus))
				sh->status = WEXITSTATUS(wstatus);
		}
		if (full_path != sh->args[0])
			free(full_path);
	}
	else
	{
		if (sh->interactive)
			_fprintf(STDERR_FILENO, "%s: command not found\n", sh->args[0]);
		else
			_fprintf(STDERR_FILENO, "%s: %d: %s: not found\n",
					 "./hsh", *curr_line, sh->args[0]);
		sh->status = 127;
	}
}

/**
 * execute_command - impliment a cmd
 * @sh: Shell structure's pointer
 * @curr_line: Current line numb's pointer
 */
void execute_command(shell *sh, int *curr_line)
{
	int t;
	char *alias_value;

	t = builtin_command(sh);
	if (t == sh->num_builtins)
	{
		/* TODO: adds up aliases we have n also args */
		alias_value = get_alias_value(sh, sh->args[0]);
		if (alias_value)
			sh->args[0] = alias_value;

		external_command(sh, curr_line);
	}
	(*curr_line)++;

	free(sh->args);
	sh->args = NULL;
}

/**
 * process_command - prints input and parses it also execute cmd
 * @sh: Shell structure's pointer
 * NOTE: '&&' yet isn't able to handle failed cmd
 */
void process_command(shell *sh)
{
	int u, curr_line = 1;
	char *oprs = ";|&", *saveptr, *cmd;

	read_input(sh);
	if (!sh->input)
		return;

	for (u = 0; u < sh->cmd_count; u++)
	{
		cmd = _strtok_r(sh->input[u], oprs, &saveptr);
		while (cmd != NULL)
		{
			/* We then parse cmd and its args */
			parse_command(sh, cmd);
			/* look if sh->args[0] is NULL or has empty string */
			if (sh->args[0] && sh->args[0][0])
				execute_command(sh, &curr_line);

			/* Look for || and earlier cmd success */
			if (saveptr[0] == '|' && sh->status == 0)
				break;
			/* Look for && n earlier cmd failed */
			if (saveptr[0] == '&' && sh->status != 0)
				break;

			cmd = _strtok_r(NULL, oprs, &saveptr);
		}
	}

	sh->cmd_count = 0;

	if (sh->input)
		free_double(&sh->input);
}
