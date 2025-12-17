#ifndef HEADER_H
#define HEADER_H

#include <ctype.h>
#include <stdio.h>

#define SUCCESS 1
#define FAILURE 0
#define EMPTY   -1

#define HASH_SIZE 27

// Linked list to store file names 
typedef struct file_node
{
    char file_name[50];
    struct file_node *link;
} file_list;

// Sub node 
typedef struct sub_node
{
    char file_name[50];
    int word_count;
    struct sub_node *link;
} sub_node;

// Main node
typedef struct main_node
{
    char word[50];
    int file_count;
    struct main_node *link;
    sub_node *sub_link;
} main_node;

// Hash table structure 
typedef struct hashtable
{
    main_node *table[HASH_SIZE];
} hash_table_t;

//Global hash table object 
extern hash_table_t ht;

// Validation 
int validate_args(int argc, char *argv[]);

// Helper functions 
int insert_last(file_list **head, char *fname);
int get_index(char *word);
void normalize_word(char *word);

main_node *find_main_node(main_node *head, char *word);
sub_node  *find_sub_node(sub_node *head, char *fname);
main_node *create_main_node(char *word, char *fname);
sub_node  *create_sub_node(char *fname);

// Operations 
int create_database(file_list *head);
int display_database(void);
int search_database(void);
int update_database(void);
int save_database(void);

// Memory cleanup functions
void free_sub_nodes(sub_node *head);
void free_main_nodes(main_node *head);
void free_hash_table(void);
void free_file_list(file_list *head);


#endif
