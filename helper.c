#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"


//insert at last operation for validation function
int insert_last(file_list **head, char *fname)
{
    file_list *new = malloc(sizeof(file_list));                                 
    if (!new)
        return FAILURE;

    strcpy(new->file_name, fname);
    new->link = NULL;

    if (*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    file_list *temp = *head;
    while (temp->link)
        temp = temp->link;

    temp->link = new;
    return SUCCESS;
}


//helper function for create database to get the indexof word
int get_index(char *word)
{
    if (isalpha(word[0]))
        return tolower(word[0]) - 'a';
    else
        return 26;
}


//function convert the word to lowercase and numbers only
void normalize_word(char *word)
{
    int i, j = 0;
    char temp[50];

    for (i = 0; word[i]; i++)
    {
        if (isalnum(word[i]))
            temp[j++] = tolower(word[i]);
    }
    temp[j] = '\0';
    strcpy(word, temp);
}

//function to traverse the main node
main_node *find_main_node(main_node *head, char *word)
{
    while (head)
    {
        if (strcmp(head->word, word) == 0)
            return head;
        head = head->link;
    }
    return NULL;
}

//function to traverse the sub node
sub_node *find_sub_node(sub_node *head, char *fname)
{
    while (head)
    {
        if (strcmp(head->file_name, fname) == 0)
            return head;
        head = head->link;
    }
    return NULL;
}

//fuction to create the main node
main_node *create_main_node(char *word, char *fname)
{
    main_node *m = malloc(sizeof(main_node));
    if (!m)
        return NULL;

    strcpy(m->word, word);
    m->file_count = 1;
    m->link = NULL;

    m->sub_link = create_sub_node(fname);
    return m;
}

//function to create the sub node
sub_node *create_sub_node(char *fname)
{
    sub_node *s = malloc(sizeof(sub_node));
    if (!s)
        return NULL;

    strcpy(s->file_name, fname);
    s->word_count = 1;
    s->link = NULL;
    return s;
}

//memmory handling 
void free_sub_nodes(sub_node *head)
{
    sub_node *temp;

    while (head)
    {
        temp = head;
        head = head->link;
        free(temp);
    }
}

void free_main_nodes(main_node *head)
{
    main_node *temp;

    while (head)
    {
        free_sub_nodes(head->sub_link);                             // free sub nodes first
        temp = head;
        head = head->link;
        free(temp);
    }
}


void free_hash_table(void)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (ht.table[i])
        {
            free_main_nodes(ht.table[i]);
            ht.table[i] = NULL;
        }
    }
}


void free_file_list(file_list *head)
{
    file_list *temp;

    while (head)
    {
        temp = head;
        head = head->link;
        free(temp);
    }
}





