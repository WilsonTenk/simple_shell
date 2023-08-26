#include "main.h"

/**
 * main - Our shell program entry point
 * @argc: Numb of args passed to our program
 * @argv: Array strings of the arguments passed to our program
 * Return: Command executed's earlier status
 */
int main(int argc, char **argv)
{
	shell wt = {0};

	init_shell(&wt, argc, argv);

	while (wt.run)
	{
		if (wt.interactive)
			_printf("$ ");

		process_command(&wt);
	}

	free_shell(&wt);
	return (wt.status);
}

/**
 * init_shell - We are initializing our shell structure
 * @argc: Numb arguments we passed to our program
 * @argv: The array/string/ of the arguments we passed
 * @wt: Shell structure's pointer
 */
void init_shell(shell *wt, int argc, char **argv)
{
	cmd *builtins = get_builtins();

	wt->builtins = builtins;
	wt->run = 1;
	wt->argc = argc;
	wt->argv = argv;

	while (builtins[wt->num_builtins].name)
		wt->num_builtins++;

	wt->interactive = isatty(STDIN_FILENO) && argc == 1;
}

/**
 * free_shell - We then freed thw  memory allocated for the shell struct
 * @wt: Shell structure we will free
 */
void free_shell(shell *wt)
{
	int t;

	if (wt->input)
		free_double(&wt->input);

	if (wt->args)
		free(wt->args);

	if (wt->environ_copy)
		free_double(&wt->environ_copy);

	for (t = 0; wt->aliases[t].name; t++)
	{
		if (wt->aliases[t].name)
			free(sh->aliases[t].name);
		if (wt->aliases[t].value)
			free(sh->aliases[t].value);
	}
}
