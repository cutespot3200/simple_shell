#include "shell.h"

alias_t *add_alias(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);
list_t *add_apex(list_t **head, char *dir);
void free_list(list_t *head);

/**
 * add_alias - Include node to  end of a alias_t linked list.
 * @head: Pointer to head of list_t list.
 * @name: Nomenclature of new alias to be added.
 * @value: Val of the new alias to be added.
 *
 * Return: Where error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */
alias_t *add_alias(alias_t **head, char *name, char *value)
{
	alias_t *recent_node = malloc(sizeof(alias_t));
	alias_t *end;

	if (!recent_node)
		return (NULL);

	recent_node->next = NULL;
	recent_node->name = malloc(sizeof(char) * (_strlen(name) + 1));
	if (!recent_node->name)
	{
		free(recent_node);
		return (NULL);
	}
	recent_node->value = value;
	_strcpy(recent_node->name, name);

	if (*head)
	{
		end = *head;
		while (end->next != NULL)
			end = end->next;
		end->next = recent_node;
	}
	else
		*head = recent_node;

	return (recent_node);
}

/**
 * add_apex - Include node to the end of a list_t linked list.
 * @head: Pointer to head of list_t list.
 * @dir: Directory path for  recent node to contain.
 *
 * Return: Where  error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */
list_t *add_apex(list_t **head, char *dir)
{
	list_t *recent_node = malloc(sizeof(list_t));
	list_t *end;

	if (!recent_node)
		return (NULL);

	recent_node->dir = dir;
	recent_node->next = NULL;

	if (*head)
	{
		end = *head;
		while (end->next != NULL)
			end = end->next;
		end->next = recent_node;
	}
	else
		*head = recent_node;

	return (recent_node);
}

/**
 * free_alias_list - Free alias_t linked list.
 * @head: Head of alias_t list.
 */
void free_alias_list(alias_t *head)
{
	alias_t *next;

	while (head)
	{
		next = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = next;
	}
}

/**
 * free_list - Free list_t linked list.
 * @head: Head of list_t list.
 */
void free_list(list_t *head)
{
	list_t *next;

	while (head)
	{
		next = head->next;
		free(head->dir);
		free(head);
		head = next;
	}
}
