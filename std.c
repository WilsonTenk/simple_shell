#include "main.h"
#include <stdio.h>

#define MAX_ARGS 10

void process_variables(shell *wt, char **args);

/**
 * read_line - This read a line from stdin
 * @fd: The fiile descriptor to read from
 * @wt: Shell structure's pointer
 * Return: Pointer to read line, or NULL EOF is reached
 */
char *read_line(shell *wt, int fd)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = _getline(&line, &len, fd);

	if (nread == -1)
	{
		wt->run = 0;
		free(line);
		return (NULL);
	}

	if (nread > 0 && line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}

/**
 * read_input - This read input from user and stores it in a shell struct
 * @wt: Shell structure's pointer
 */
void read_input(shell *wt)
{
	char *cmd;
	size_t old_size, new_size;
	int fd = STDIN_FILENO;

	if (wt->argc > 1)
	{
		fd = open(wt->argv[1], O_RDONLY);
		if (fd == -1)
		{
			_fprintf(STDERR_FILENO, "%s: 0: Can't open %s\n",
					 "./hsh", wt->argv[1]);
			wt->status = 127;
			wt->run = 0;
			return;
		}
	}

	while ((cmd = read_line(wt, fd)))
	{
		old_size = sizeof(char *) * (wt->cmd_count + 1);
		new_size = sizeof(char *) * (wt->cmd_count + 2);

		wt->input = _realloc(wt->input, old_size, new_size);
		wt->input[wt->cmd_count] = cmd;
		wt->input[wt->cmd_count + 1] = NULL;
		wt->cmd_count++;

		if (wt->interactive)
			break;
	}

	if (fd != STDIN_FILENO)
		close(fd);
}

/**
 * parse_command - This parses a cmd string into array of args
 * @wt: Shell structure's pointer
 * @cmd: Command string to parsed
 * Return: Array of args pointers, or NULL if cmd is empty
 */
void parse_command(shell *wt, char *cmd)
{
	int t = 0;
	char **args = malloc(MAX_ARGS * sizeof(char *));
	char *arg, *start, *end;

	if (!args)
	{
		_fprintf(STDERR_FILENO, "Error: mem allocation failed\n");
		exit(EXIT_FAILURE);
	}
	if (!cmd || !*cmd)
	{
		wt->args = args;
		return;
	}
	arg = _strtok(cmd, " \t\n\r");
	while (arg)
	{
		/* Check for the comment */
		if (arg[0] == '#')
			break;
		/* Clear the double quotes */
		start = arg;
		end = start + _strlen(start) - 1;
		if (start[0] == '"' && end[0] == '"')
			(end[0] = '\0', start++);

		args[t] = start;
		if (++t >= MAX_ARGS)
		{
			_fprintf(STDERR_FILENO, "Error: Too many args\n");
			exit(EXIT_FAILURE);
		}
		arg = _strtok(NULL, " \t\n\r");
	}
	args[t] = NULL;
	process_variables(wt, args);
	wt->args = args;
}

/**
 * process_variables - This process variables in the shell
 * @wt: Shell structure's pointer
 * @args: Array args
 */
void process_variables(shell *wt, char **args)
{
	int t = 0;
	char *arg_value;
	static char status_str[12], pid_str[12];

	for (; args[t]; t++)
	{
		if (args[t][0] != '$')
			continue;

		if (_strcmp(args[t], "$?", -1) == 0)
		{
			_sprintf(status_str, "%d", wt->status);
			args[t] = status_str;
		}
		else if (_strcmp(args[t], "$$", 2) == 0)
		{
			_sprintf(pid_str, "%d", getpid());
			args[t] = pid_str;
		}
		else if (args[t][1] == '\0' || args[t][1] == ' ')
		{
			args[t] = "$";
		}
		else
		{
			arg_value = _getenv(args[t] + 1);
			if (arg_value)
				args[t] = arg_value;
			else
				args[t] = "";
		}
	}
}
