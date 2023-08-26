#ifndef MAIN_H
#define MAIN_H

/* ----including the files for Shell project---- */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include "sys/stat.h"
#include "sys/types.h"
#include <sys/wait.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <stdio.h>

#define BUFFER_SIZE 2048
#define ALIASES_SIZE 100

typedef struct shell shell;


/**
 * struct cmd - Struct to capture a cmd name n fxn pointer
 * @name: Command name string’s pointer
 * @func: Pointer to the fxn associated with the cmd
 */
typedef struct cmd
{
	char *name;
	void (*func)(shell *);
} cmd;


/**
 * struct alias - Structure for capturing an alias name n value
 * @name: Pointer to alias name here
 * @value: Pointer to alias value here
 */
typedef struct alias
{
	char *name;
	char *value;
} alias;


/**
 * struct shell - Structure has info about the shell environment
 * @input: The array of strings has cmd from the user input
 * @args: The array of strings has args from the user input
 * @environ_copy: The array of strings that has copy of environ
 * @builtins: Pointer to an array the builtin commands
 * @num_builtins: Count of builtin cmd
 * @cmd_count: Count of cmd in the user input
 * @status: Exit status of previous cmd executed
 * @interactive: Boolean whether the program works interactively or not
 * @builtins: Pointer to an array of havin about the builtin cmd
 * @run: Boolean if or not the shell should continue to run
 * @aliases: Array of alias structures having user-defined aliases
 * @argc: Arguments to be counted
 * @argv: Program arguments
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


extern char **environ;

/* ---- string_tools --- */
int _strlen(char *s);
void reverse_str(char s[]);
int _stoa(char *s, char *buf);
int _strcmp(const char *s1, const char *s2, size_t n);
void *_memcpy(void *dest, const void *src, size_t n);
char *_strdup(const char *s);
char *_strtok(char *str, char *delim);
char *_strtok_r(char *str, const char *delim, char **saveptr);
char *_strchr(char *s, int c);

/* --- here main--- */
void init_shell(shell *wt, int argc, char **argv);
void free_shell(shell *wt);


/* ---- the Sprintf---- */
void _printf(const char *fmt, ...);
void _fprintf(int fd, const char *fmt, ...);
void _sprintf(char *str, const char *fmt, ...);

/* --- the execution--- */
char *find_command(char *command);
int builtin_command(shell *wt);
void process_command(shell *wt);
void execute_command(shell *wt, int *curr_line);
void external_command(shell *wt, int *curr_line);

/* ---- the num_tools--- */
int _atoi(const char *str);
int _itoa(long n, char s[], int base, int sign);
void *_realloc(void *ptr, ssize_t old_size, ssize_t new_size);
void free_double(char ***ptr);
int num_val(char *str);

/* --- the input---*/
void read_input(shell *wt);
void parse_command(shell *wt, char *cmd);


/* --- the builtins functions---*/
cmd *get_built(void);
int num_built(void);
cmd *get_builtins(void);
char *get_alias_value(shell *wt, char *name);
void cmd_alias(shell *wt);
void update_environment(shell *wt, char *env_var);


/* ---- the system tools----*/
ssize_t _getline(char **lineptr, size_t *n, int fd);
char *_getenv(const char *name);
char **copy_environ(void);

/* ----- the builtins---- */

void cmd_setenv(shell *wt);
void cmd_unsetenv(shell *wtwrite);
cmd *get_builtins(void);

#endif
