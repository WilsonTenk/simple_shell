#ifndef MAIN_H
#define MAIN_H

/* ---------------------------- Includes ---------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include "sys/stat.h"
#include "sys/types.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

/* ---------------------------- Definitions ---------------------------- */

#define BUFFER_SIZE 2048
#define ALIASES_SIZE 100

typedef struct shell shell;

/* -------------- Struct Definitions ------------------------ */

/**
 * struct cmd - Stores command name and associated function pointer
 * @name: Pointer to the command name string
 * @func: Pointer to the function associated with the command
 */
typedef struct cmd
{
	char *name;
	void (*func)(shell *);
} cmd;

/**
 * struct alias - Stores alias name and its value
 * @name: Pointer to the alias name
 * @value: Pointer to the alias value
 */
typedef struct alias
{
	char *name;
	char *value;
} alias;

/**
 * struct shell - Contains information about the shell environment
 * @input: Array of strings with commands from user input
 * @args: Array of strings with arguments from user input
 * @environ_copy: Array of strings with a copy of the environment
 * @builtins: Pointer to array havin  inf about built-in commands
 * @num_builtins: Count of built-in commands
 * @cmd_count: Count of commands in user input
 * @status: Exit status of the last command executed
 * @interactive: Boolean indicating if the program is running interactively
 * @run: Boolean indicating if the shell should continue running
 * @aliases: Array of alias structures containing user-defined aliases
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 */
struct shell
{
	char **input;
	char **args;
	char **environ_copy;
	int num_builtins;
	int cmd_count;
	int status;
	int run;
	int argc;
	char **argv;
	int interactive;
	cmd *builtins;
	alias aliases[ALIASES_SIZE];
};

/* -------------------- External Variables------------------------- */

extern char **environ;

/* ---------------------------- String Tools ---------------------------- */

int _strlen(char *s);
void reverse_str(char s[]);
int _stoa(char *s, char *buf);
int _strcmp(const char *s1, const char *s2, size_t n);
void *_memcpy(void *dest, const void *src, size_t n);
char *_strdup(const char *s);
char *_strtok(char *str, char *delim);
char *_strtok_r(char *str, const char *delim, char **saveptr);
char *_strchr(char *s, int c);

/* ---------------------------- Main ---------------------------- */

void init_shell(shell *sh, int argc, char **argv);
void free_shell(shell *sh);

/* ---------------------------- Sprintf ---------------------------- */

void _printf(const char *fmt, ...);
void _fprintf(int fd, const char *fmt, ...);
void _sprintf(char *str, const char *fmt, ...);

/* ---------------------------- Execution ---------------------------- */

char *find_command(char *command);
int builtin_command(shell *sh);
void process_command(shell *sh);
void execute_command(shell *sh, int *curr_line);
void external_command(shell *sh, int *curr_line);

/* ---------------------------- Number Tools ---------------------------- */

int _atoi(const char *str);
int _itoa(long n, char s[], int base, int sign);
void *_realloc(void *ptr, ssize_t old_size, ssize_t new_size);
void free_double(char ***ptr);
int is_num(char *str);

/* ---------------------------- Input ---------------------------- */

void read_input(shell *sh);
void parse_command(shell *sh, char *cmd);

/* ------------ Builtins Functions ------------------------ */

cmd *get_built(void);
int num_built(void);
cmd *get_builtins(void);
char *get_alias_value(shell *sh, char *name);
void cmd_alias(shell *sh);
void update_environment(shell *sh, char *env_var);

/* ---------------------------- System Tools ---------------------------- */

ssize_t _getline(char **lineptr, size_t *n, int fd);
char *_getenv(const char *name);
char **copy_environ(void);

/* ---------------------------- Builtins ---------------------------- */

void cmd_setenv(shell *sh);
void cmd_unsetenv(shell *sh);
cmd *get_builtins(void);

#endif
