/*
NAME : Azif Nizam
BATCH : 25021A
REG NO : 25021A_008 
DATE : 15-12-2025

PROJECT NAME : INVERTED SEARCH
DESCRIPTION : An inverted search engine implemented in C that indexes and retrieves words from multiple text files
              using a hash-table–based data structure. Builds an in-memory inverted index by parsing input files 
              provided via command-line arguments, mapping each unique word to the list of files in which it appears 
              along with occurrence counts. Utilizes a fixed-size hash table with collision handling through linked 
              lists, hierarchical main and sub-node structures, and word normalization for consistent indexing. 
              Supports database creation, search, display, update, and persistent storage through structured file 
              serialization and deserialization. Designed with modular architecture to ensure clear separation of 
              validation, data structures, helper utilities, and core operations without reliance on external libraries.

*/


#include <stdio.h>
#include "header.h"

int c_flag = 0;                                                     // create done
int u_flag = 0;                                                     // update done

extern file_list *head;

int main(int argc, char *argv[])
{
    int option;

    if (validate_args(argc, argv) == FAILURE)
    {
        printf("## Argument validation failed\n");
        return 0;
    }

    while (1)
    {
        printf("\n-----FILE MANAGER-----");
        printf("\n1. Create Database");
        printf("\n2. Search Database");
        printf("\n3. Display Database");
        printf("\n4. Update Database");
        printf("\n5. Save Database");
        printf("\n6. Exit");
        printf("\n>> Enter your choice: ");

        scanf("%d", &option);
        printf("\n");

        switch (option)
        {
            case 1:                                                  // CREATE DATABASE
                if (c_flag == 0)
                {
                    if (create_database(head) == SUCCESS)
                    {
                        printf(">> DATABASE created successfully\n");
                    }
                    c_flag = 1;
                }
                else
                {
                    printf("## Error: Create operation not allowed again\n");
                }
                break;

            case 2:                                                 // SEARCH DATABASE
                search_database();
                break;

            case 3:                                                 // DISPLAY DATABASE
                display_database();
                break;

            case 4:                                                 // UPDATE DATABASE
                if (c_flag == 0 && u_flag == 0)
                {
                    if(update_database() == SUCCESS)
                    {
                        printf(">> DATABASE updated successfully\n");
                    }
                    u_flag = 1;
                }
                else
                {
                    printf("## Error: Update operation not allowed\n");
                }
                break;

            case 5:                                                 // SAVE DATABASE
                save_database();
                break;

            case 6:
                free_hash_table();
                free_file_list(head);
                return 0;

            default:
                printf("## Invalid choice\n");
        }
    }
}
