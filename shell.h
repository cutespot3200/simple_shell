#ifndef _SHELL_H_
#define _SHELL_H_


#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <stddef.h>


#define EXIT -3
#define END_OF_FILE -2


/* Glo hist count */
int hist;

/* Glo env */
extern char **environ;

/* Glo program name */
char *name;

/**
 * struct builtin_s -  struct type definin builtins command.
 * @name: nomenclature of the builtin command.
 * @f:  func pointer to  builtin command function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv, char **front);
} builtin_t;

/**
 * struct list_s -  struct type definin linked list.
 * @dir:  directory path.
 * @next:  pointer to some other struct list_s.
 */
typedef struct list_s
{
	char *dir;
	struct list_s *next;
} list_t;

/**
 * struct alias_s - struct type  definin aliases.
 * @name: nomenclature of the alias.
 * @value: value of alias.
 * @next: pointer to some other struct alias_s.
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

/* Glo aliases linked list */
alias_t *aliases;

/* Functions for String Type*/

/* Compares 2 strings */
int _strncmp(const char *s1, const char *s2, size_t n);

/* Counts the number of characters of a string */
int _strlen(const char *s);

/* Compares 2 strings */
int _strcmp(char *s1, char *s2);

/* Concatenates two strings */
char *_strcat(char *dest, const char *src);

/*returns the length of the starting substring of the string*/
int _strspn(char *s, char *accept);

/*finds the first occurrence of a character in a string*/
char *_strchr(char *s, char c);

/*function appends the first count characters of string2 to string1*/
char *_strncat(char *dest, const char *src, size_t n);

/*copies the string pointed by source*/
char *_strcpy(char *dest, const char *src);


/*PRINCIPAL ASSISTANTS*/
char *_ita(int num);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
void free_list(list_t *head);
int implement(char **args, char **front);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char *go_loc(char *command);
char **_strtok(char *line, char *delim);
list_t *go_paths_dir(char *path);

/*BUILTIN ASSISTANTS*/
void free_env(void);
char **_dupenv(void);
char **_goenv(const char *var);

/*HELPERS*/
void aid_history(void);
void aid_all(void);
void aid_setenv(void);
void aid_env(void);
void aid_unsetenv(void);
void aid_alias(void);
void aid_help(void);
void aid_exit(void);
void aid_cd(void);

/*LINKEDLIST ASSISTANTS*/
list_t *add_apex(list_t **head, char *dir);
alias_t *add_alias(alias_t **head, char *name, char *value);
void free_list(list_t *head);
void free_alias_list(alias_t *head);

/*INPUT ASSISTANTS*/
void free_args(char **args, char **front);
char **restore_aliases(char **args);
void hold_line(char **line, ssize_t read);
int hold_args(int *exe_ret);
void vari_rep(char **args, int *exe_ret);
int dub_args(char **args, char **front, int *exe_ret);
int check_args(char **args);
int run_args(char **args, char **front, int *exe_ret);
char *go_args(char *line, int *exe_ret);

/*ERRATUM HANDLING*/
char *err_127(char **args);
int make_error(char **args, int err);
char *err_126(char **args);
char *err_1(char **args);
char *err_env(char **args);
char *err_2_syntax(char **args);
char *err_2_exit(char **args);
char *err_2_cd(char **args);

/*BUILTINS*/
int shello_help(char **args, char __attribute__((__unused__)) **front);
int shello_exit(char **args, char **front);
int shello_setenv(char **args, char __attribute__((__unused__)) **front);
int shello_env(char **args, char __attribute__((__unused__)) **front);
int shello_cd(char **args, char __attribute__((__unused__)) **front);
int shello_alias(char **args, char __attribute__((__unused__)) **front);
int shello_unsetenv(char **args, char __attribute__((__unused__)) **front);
int (*get_builtin(char *command))(char **args, char **front);

int proc_file_command(char *file_path, int *exe_ret);
#endif /*_SHELL_H_*/
