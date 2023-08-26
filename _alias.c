#include "main.h"

/**
 * willy_print_aliases - Prints all aliases in a shell struct
 * @sh: Pointer to the shell structure
 */
void willy_print_aliases(shell *sh)
{
	int j;

	for (j = 0; j < ALIASES_SIZE; j++)
	{
		if (sh->aliases[j].name != NULL)
		{
			willy_printf("%s='%s'\n", sh->aliases[j].name, sh->aliases[j].value);
		}
	}
}

/**
 * willy_print_alias - Prints the alias of a given name
 * @sh: Pointer to shell struct
 * @name: Name of the alias to be printed with value
 */
void willy_print_alias(shell *sh, char *name)
{
	int j;

	for (j = 0; j < ALIASES_SIZE; j++)
	{
		if (sh->aliases[j].name && willy_strcmp(sh->aliases[j].name, name, -1) == 0)
		{
			willy_printf("%s='%s'\n", sh->aliases[j].name, sh->aliases[j].value);
		}
	}
}

/**
 * willy_set_alias - Set an alias in the shell
 * @sh: Pointer to the shell struct
 * @name: Name of the alias
 * @value: Value of the alias
 */
void willy_set_alias(shell *sh, char *name, char *value)
{
	char *new_value;
	int i, j = 0, found = 0;
	int len = willy_strlen(value);

	/* Create a new string without quotes */
	new_value = willy_malloc(len + 1);

	for (i = 0; i < len; i++)
	{
		if (value[i] != '"' && value[i] != '\'')
			new_value[j++] = value[i];
	}
	new_value[j] = '\0';
	/* Check if the alias already exists */
	for (j = 0; j < ALIASES_SIZE; j++)
	{
		if (sh->aliases[j].name && willy_strcmp(sh->aliases[j].name, name, -1) == 0)
		{
			willy_free(sh->aliases[j].value);
			sh->aliases[j].value = willy_strdup(new_value);
			found = 1;
			break;
		}
	}
	/* If the alias doesn't exist, add it */
	if (!found)
	{
		for (j = 0; j < ALIASES_SIZE; j++)
		{
			if (sh->aliases[j].name == NULL)
			{
				sh->aliases[j].name = willy_strdup(name);
				sh->aliases[j].value = willy_strdup(new_value);
				break;
			}
		}
	}

	willy_free(new_value);
}

/**
 * willy_cmd_alias - Set or print aliases.
 * @sh: Pointer to the shell struct
 */
void willy_cmd_alias(shell *sh)
{
	int i = 1, index, j;
	char name[ALIASES_SIZE];
	char *arg, *value, *equal_sign;

	/* Use an early return if the first argument is NULL */
	if (sh->args[i] == NULL)
	{
		willy_print_aliases(sh);
		return;
	}

	for (; sh->args[i] != NULL; i++)
	{
		arg = sh->args[i];
		equal_sign = NULL;
		/* Search for the '=' character in arg */
		for (index = 0; arg[index] != '\0'; index++)
		{
			if (arg[index] == '=')
			{
				equal_sign = &arg[index];
				break;
			}
		}

		if (equal_sign)
		{
			index = equal_sign - arg;
			/* Copy characters from arg to name */
			for (j = 0; j < index && j < ALIASES_SIZE - 1; j++)
				name[j] = arg[j];

			name[j] = '\0';
			value = equal_sign + 1;
			willy_set_alias(sh, name, value);
		}
		else
			willy_print_alias(sh, arg);
	}
}

/**
 * willy_get_alias_value - Retrieve the value of an alias from a shell struct
 * @sh: Pointer to the shell structure
 * @name: Name of the alias to retrieve
 * Return: The value of the alias, or NULL if not found
 * NOTE: hash-tables to optimize
 */
char *willy_get_alias_value(shell *sh, char *name)
{
	int i;
	char *value = NULL;
	alias *curr_alias;

	for (i = 0; i < ALIASES_SIZE; i++)
	{
		curr_alias = &sh->aliases[i];
		if (curr_alias->name && willy_strcmp(curr_alias->name, name, -1) == 0)
		{
			value = curr_alias->value;
			break;
		}
	}

	/* Check if the value is already an alias */
	for (i = 0; value && i < ALIASES_SIZE; i++)
	{
		curr_alias = &sh->aliases[i];
		if (curr_alias->name && willy_strcmp(curr_alias->name, value, -1) == 0)
		{
			value = curr_alias->value;
			break;
		}
	}

	return (value);
}
