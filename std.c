#include "main.h"
#include <stdio.h>

#define MAX_ARGS 10

void process_variables(shell *sh, char **args);

/**
 * read_line - Here Read a line from stdin
 * @fd: Here File descriptor to read from
 * @sh: Pointer to the shell structure
 * Return: Pointer to the line read, or NULL if EOF is reached
 */
char *read_line(shell *sh, int fd)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = my_getline(&line, &len, fd);

	if (nread == -1)
	{
		sh->run = 0;
		free(line);
		return (NULL);
	}

	if (nread > 0 && line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}

/**
 * read_input - Read input from the user and store it in a shell struct
 * @sh: Pointer to the shell structure
 */
void read_input(shell *sh)
{
	char *cmd;
	size_t old_size, new_size;
	int fd = STDIN_FILENO;

	if (sh->argc > 1)
	{
		fd = my_open(sh->argv[1], O_RDONLY);
		if (fd == -1)
		{
			my_dprintf(STDERR_FILENO, "%s: 0: Can't open %s\n",
					 "./hsh", sh->argv[1]);
			sh->status = 127;
			sh->run = 0;
			return;
		}
	}

	while ((cmd = read_line(sh, fd)))
	{
		old_size = sizeof(char *) * (sh->cmd_count + 1);
		new_size = sizeof(char *) * (sh->cmd_count + 2);

		sh->input = my_realloc(sh->input, old_size, new_size);
		sh->input[sh->cmd_count] = cmd;
		sh->input[sh->cmd_count + 1] = NULL;
		sh->cmd_count++;

		if (sh->interactive)
			break;
	}

	if (fd != STDIN_FILENO)
		my_close(fd);
}

/**
 * parse_command - Parse a command string into an array of arguments
 * @sh: Pointer to the shell structure
 * @cmd: The command string to be parsed
 * Return: Pointer to the array of arguments, or NULL if the command is empty
 */
void parse_command(shell *sh, char *cmd)
{
	int i = 0;
	char **args = my_malloc(MAX_ARGS * sizeof(char *));
	char *arg, *start, *end;

	if (!args)
	{
		my_dprintf(STDERR_FILENO, "Error: memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	if (!cmd || !*cmd)
	{
		sh->args = args;
		return;
	}
	arg = my_strtok(cmd, " \t\n\r");
	while (arg)
	{
		/* Check for comment */
		if (arg[0] == '#')
			break;
		/* Remove double quotes */
		start = arg;
		end = start + my_strlen(start) - 1;
		if (start[0] == '"' && end[0] == '"')
			(end[0] = '\0', start++);

		args[i] = start;
		if (++i >= MAX_ARGS)
		{
			my_dprintf(STDERR_FILENO, "Error: too many arguments\n");
			exit(EXIT_FAILURE);
		}
		arg = my_strtok(NULL, " \t\n\r");
	}
	args[i] = NULL;
	process_variables(sh, args);
	sh->args = args;
}

/**
 * process_variables - Process variables in the shell
 * @sh: Pointer to the shell structure
 * @args: Array of arguments
 */
void process_variables(shell *sh, char **args)
{
	int i = 0;
	char *arg_value;
	static char status_str[12], pid_str[12];

	for (; args[i]; i++)
	{
		if (args[i][0] != '$')
			continue;

		if (my_strcmp(args[i], "$?", -1) == 0)
		{
			my_sprintf(status_str, "%d", sh->status);
			args[i] = status_str;
		}
		else if (my_strcmp(args[i], "$$", 2) == 0)
		{
			my_sprintf(pid_str, "%d", my_getpid());
			args[i] = pid_str;
		}
		else if (args[i][1] == '\0' || args[i][1] == ' ')
		{
			args[i] = "$";
		}
		else
		{
			arg_value = my_getenv(args[i] + 1);
			if (arg_value)
				args[i] = arg_value;
			else
				args[i] = "";
		}
	}
}
