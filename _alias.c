#include "main.h"

/**
 * print_aliases - Output aliases in a shell struct
 * @sh: Shell structure will be pointed to
 */
void print_aliases(shell *sh)
{
	int t;

	for (t = 0; t < ALIASES_SIZE; t++)
	{
		if (sh->aliases[t].name != NULL)
		{
			_printf("%s='%s'\n", sh->aliases[t].name, sh->aliases[t].value);
		}
	}
}

/**
 * print_alias - A given name of an alias is printed
 * @sh: Struct will be pointed to
 * @name: Alias name to be printed with value
 */
void print_alias(shell *sh, char *name)
{
	int t;

	for (t = 0; t < ALIASES_SIZE; t++)
	{
		if (sh->aliases[t].name && _strcmp(sh->aliases[t].name, name, -1) == 0)
		{
			_printf("%s='%s'\n", sh->aliases[t].name, sh->aliases[t].value);
		}
	}
}

/**
 * set_alias - Alias set here
 * @sh: Shell struct will be pointed at
 * @name: Is the alias name
 * @value: The value of the  alias
 */
void set_alias(shell *sh, char *name, char *value)
{
	char *new_value;
	int u, t = 0, found = 0;
	int len = _strlen(value);

	/* ----New string created without quotes---- */
	new_value = malloc(len + 1);

	for (u = 0; u < len; u++)
	{
		if (value[u] != '"' && value[u] != '\'')
			new_value[t++] = value[u];
	}
	new_value[t] = '\0';
	/* Does alias already exists */
	for (t = 0; t < ALIASES_SIZE; t++)
	{
		if (sh->aliases[t].name && _strcmp(sh->aliases[t].name, name, -1) == 0)
		{
			free(sh->aliases[t].value);
			sh->aliases[t].value = _strdup(new_value);
			found = 1;
			break;
		}
	}
	/* In case alias doesn't exist, add it */
	if (!found)
	{
		for (t = 0; t < ALIASES_SIZE; t++)
		{
			if (sh->aliases[t].name == NULL)
			{
				sh->aliases[t].name = _strdup(name);
				sh->aliases[t].value = _strdup(new_value);
				break;
			}
		}
	}

	free(new_value);
}

/**
 * cmd_alias - To set aliases.
 * @sh: Shell struct get pointed to
 */
void cmd_alias(shell *sh)
{
	int u = 1, index, t;
	char name[ALIASES_SIZE];
	char *arg, *value, *equal_sign;

	/* Utilize the early return if the first argument is NULL */
	if (sh->args[u] == NULL)
	{
		print_aliases(sh);
		return;
	}

	for (; sh->args[u] != NULL; u++)
	{
		arg = sh->args[u];
		equal_sign = NULL;
		/* Lookup for the '=' char in arg */
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
			/* Copy chars from arg to name */
			for (t = 0; t < index && t < ALIASES_SIZE - 1; t++)
				name[t] = arg[t];

			name[t] = '\0';
			value = equal_sign + 1;
			set_alias(sh, name, value);
		}
		else
			print_alias(sh, arg);
	}
}

/**
 * get_alias_value - Get back value of alias from a shell struct
 * @sh: Pointer going to  shell structure
 * @name: Our alias name to retrieve
 * Return: Value of alias, or NULL if not found
 * NOTE: optimize the hash-table
 */
char *get_alias_value(shell *sh, char *name)
{
	int u;
	char *value = NULL;
	alias *curr_alias;

	for (u = 0; u < ALIASES_SIZE; u++)
	{
		curr_alias = &sh->aliases[u];
		if (curr_alias->name && _strcmp(curr_alias->name, name, -1) == 0)
		{
			value = curr_alias->value;
			break;
		}
	}

	/* Lookup value is already an alias */
	for (u = 0; value && u < ALIASES_SIZE; u++)
	{
		curr_alias = &sh->aliases[u];
		if (curr_alias->name && _strcmp(curr_alias->name, value, -1) == 0)
		{
			value = curr_alias->value;
			break;
		}
	}

	return (value);
}
