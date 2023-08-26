#include "main.h"

/**
 * main - Entry point for the shell program
 * @argc: The number of arguments passed to the program
 * @argv: An array[string] of the arguments passed to the program
 * Return: The status code of the last command executed
 */
int main(int argc, char **argv)
{
	shell sh = {0};

	willy_init_shell(&sh, argc, argv);

	while (sh.run)
	{
		if (sh.interactive)
			willy_printf("$ ");

		willy_process_command(&sh);
	}

	willy_free_shell(&sh);
	return (sh.status);
}

/**
 * willy_init_shell - Initialize the shell struct
 * @sh: Pointer to the shell structure
 * @argc: The number of arguments passed to the program
 * @argv: An array[string] of the arguments passed to the program
 */
void willy_init_shell(shell *sh, int argc, char **argv)
{
	cmd *builtins = willy_get_builtins();

	sh->builtins = builtins;
	sh->run = 1;
	sh->argc = argc;
	sh->argv = argv;

	while (builtins[sh->num_builtins].name)
		sh->num_builtins++;

	sh->interactive = willy_isatty(STDIN_FILENO) && argc == 1;
}

/**
 * willy_free_shell - Frees the memory allocated for a shell structure
 * @sh: The shell structure to be freed
 */
void willy_free_shell(shell *sh)
{
	int i;

	if (sh->input)
		willy_free_double(&sh->input);

	if (sh->args)
		willy_free(sh->args);

	if (sh->environ_copy)
		willy_free_double(&sh->environ_copy);

	for (i = 0; sh->aliases[i].name; i++)
	{
		if (sh->aliases[i].name)
			willy_free(sh->aliases[i].name);
		if (sh->aliases[i].value)
			willy_free(sh->aliases[i].value);
	}
}
