#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

hash_table_t ht;                                                                //hastable variable

file_list *head = NULL;                                                         //intitializing headpointer of file linked list to null


// validate the arguments
int validate_args(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("## Error: No files provided\n");
        return FAILURE;
    }

    for (int i = 1; i < argc; i++)
    {
        char *ext = strrchr(argv[i], '.');

        if (ext == NULL || strcmp(ext, ".txt") != 0)                            // Check .txt
        {
            printf("## Error: %s is not a text file\n", argv[i]);
            return FAILURE;
        }

        file_list *temp = head;                                                 // Check duplicate
        while (temp)
        {
            if (strcmp(temp->file_name, argv[i]) == 0)
            {
                printf("## Warning: Duplicate file %s ignored\n", argv[i]);
                goto skip;
            }
            temp = temp->link;
        }
 
        FILE *fp = fopen(argv[i], "r");                                         // Open file
        if (fp == NULL)
        {
            printf("## Error: Cannot open %s\n", argv[i]);
            return FAILURE;
        }

        fseek(fp, 0, SEEK_END);                                                 // Check empty
        if (ftell(fp) == 0)
        {
            printf("## Warning: %s is empty, ignored\n", argv[i]);
            fclose(fp);
            continue;
        }
        fclose(fp);

        insert_last(&head, argv[i]);                                            // Insert into list 

    skip:
        continue;
    }

    return (head == NULL) ? FAILURE : SUCCESS;
}

//create database
int create_database(file_list *head)
{

    char word[50];

    for (int i = 0; i < HASH_SIZE; i++)                                 //inittializing all links to null
        ht.table[i] = NULL;

    while (head)
    {
        FILE *fp = fopen(head->file_name, "r");                     
        if (!fp)                                                        //if opening file fails, continuw to next file
        {
            head = head->link;
            continue;
        }

        while (fscanf(fp, "%s", word) != EOF)                           //start reading words 
        {
            normalize_word(word);                                       //to convert word to lowercase
            if (strlen(word) == 0)
                continue;

            int index = get_index(word);                                //to get the index of the word       

            main_node *m = find_main_node(ht.table[index], word);       //to traverse the main node

            if (!m)                                                     //if the word is new create new main node
            {
                m = create_main_node(word, head->file_name);
                m->link = ht.table[index];
                ht.table[index] = m;
            }
            else                                                        //if word already exists
            {
                sub_node *s = find_sub_node(m->sub_link, head->file_name);  //to traverse sub node

                if (s)                                                  //if word already in file
                    s->word_count++;
                else                                                    //if word found in new file create new sub node
                {
                    s = create_sub_node(head->file_name);
                    s->link = m->sub_link;
                    m->sub_link = s;
                    m->file_count++;
                }
            }
        }

        fclose(fp);
        head = head->link;                                              //moving to next file
    }

    return SUCCESS;
}

//display database
int display_database(void)
{
    int empty_flag = 1;

    printf("\nINDEX  WORD        FILE_COUNT  FILE_NAME & WORD_COUNT\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < HASH_SIZE; i++)                                //loop to scan the hash table
    {
        if (ht.table[i] == NULL)                                       //skip empty index
            continue;

        empty_flag = 0;

        main_node *mtemp = ht.table[i];
        while (mtemp)                                                  //traverse main node
        {
            printf("%-5d  %-10s  %-10d  ", i, mtemp->word, mtemp->file_count);

            sub_node *stemp = mtemp->sub_link;
            while (stemp)                                              //traverse sub node
            {
                printf("%s %d   ", stemp->file_name, stemp->word_count);
                stemp = stemp->link;
            }

            printf("\n");
            mtemp = mtemp->link;
        }
    }

    if (empty_flag)                                                    //after scanning whole table
    {
        printf("## Database is empty\n");
        return FAILURE;
    }

    return SUCCESS;
}

//search database
int search_database(void)
{
    char word[50];

    printf(">> Enter the word to search: ");                             //read word from the user
    scanf("%s", word);

    normalize_word(word);                                             //normalizing the word to lowercase

    if (strlen(word) == 0)                                            //if word is null afer normalization
    {
        printf("## Invalid word\n");
        return FAILURE;
    }

    int index = get_index(word);                                      //calculate the index of the hash table

    if (ht.table[index] == NULL)                                      //if word not found 
    {
        printf("## Word '%s' not found\n", word);
        return FAILURE;
    }

    main_node *mtemp = ht.table[index];

    while (mtemp)                                                    //if word found traverse the main node
    {
        if (strcmp(mtemp->word, word) == 0)
        {
            printf("\n>> Word found!\n");
            printf("Word      : %s\n", mtemp->word);
            printf("File count: %d\n", mtemp->file_count);
            printf("File name and File count:\n");

            sub_node *stemp = mtemp->sub_link;
            while (stemp)                                           //traversing the subnode
            {
                printf("    %s : %d\n", stemp->file_name, stemp->word_count);
                stemp = stemp->link;
            }

            return SUCCESS;
        }

        mtemp = mtemp->link;
    }

    printf("## Word '%s' not found\n", word);
    return FAILURE;
}

//save database
int save_database(void)
{
    char fname[50];                                                 //temp buffer to store the filename

    printf(">> Enter the file name to save database: ");               //read file name
    scanf("%s", fname);

    char *ext = strrchr(fname, '.');                                //filename validation
    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("## Error: File must be a .txt file\n");
        return FAILURE;
    }

    FILE *fp = fopen(fname, "w");                                   //open the file
    if (!fp)
    {
        printf("## Error: Unable to create file %s\n", fname);
        return FAILURE;
    }

    int empty_flag = 1;

    for (int i = 0; i < HASH_SIZE; i++)                             //traversing  the hashtable
    {
        if (ht.table[i] == NULL)                                    //if index empty
            continue;

        main_node *mtemp = ht.table[i];
        while (mtemp)                                               //traverse the main node
        {
            empty_flag = 0;

            fprintf(fp, "# %d ; %s ; %d ; ", i, mtemp->word, mtemp->file_count); //print data to file

            sub_node *stemp = mtemp->sub_link;
            while (stemp)                                           //traverse sub node
            {
                fprintf(fp, "%s ; %d ; ", stemp->file_name, stemp->word_count);  //print data to file
                stemp = stemp->link;
            }

            fprintf(fp, "#\n");
            mtemp = mtemp->link;
        }
    }

    fclose(fp);

    if (empty_flag)                                                //if hashtable is empty
    {
        printf(">> Database is empty, nothing saved\n");
        return FAILURE;
    }

    printf(">> Database successfully saved to %s\n", fname);
    return SUCCESS;
}

//update database
int update_database(void)
{
    for (int i = 0; i < HASH_SIZE; i++)                             //to clear old data if exists
    ht.table[i] = NULL;


    char fname[50];                                                 //temp buffer to store the file name
    char line[256];                                                 //temp nuffer to store one full line from the file

    printf(">> Enter the database file name: ");
    scanf("%s", fname);

    FILE *fp = fopen(fname, "r");                                   //opening the file
    if (!fp)
    {
        printf("## Error: Unable to open file\n");
        return FAILURE;
    }

    while (fgets(line, sizeof(line), fp))                           //start reading data from file line by line
    {
        if (line[0] != '#')                                         //line validation 
        {
            printf("## Error: Invalid database file format\n");
            fclose(fp);
            return FAILURE;
        }

        int index, file_count;                                      //temp buffers 
        char word[50];
        char *token;

        token = strtok(line, "#;");                                 //tokenizing , converting and reading data
        index = atoi(token);                                        //read index

        token = strtok(NULL, ";");
        strcpy(word, token);                                        //read word
        normalize_word(word);

        token = strtok(NULL, ";");
        file_count = atoi(token);                                   //read file count

        main_node *m = malloc(sizeof(main_node));                   //creating main node
        if (!m)
        {
            fclose(fp);
            return FAILURE;
        }

        strcpy(m->word, word);                                      //initializing main node
        m->file_count = file_count;
        m->link = NULL;
        m->sub_link = NULL;

        if (ht.table[index] == NULL)                                //insert main node to hash table
        {
            ht.table[index] = m;
        }
        else
        {
            main_node *temp = ht.table[index];
            while (temp->link)
                temp = temp->link;
            temp->link = m;
        }

        for (int i = 0; i < file_count; i++)                        //loop to create subnode
        {
            char filename[50];
            int word_count;

            token = strtok(NULL, ";");
            strcpy(filename, token);                                //read filename

            token = strtok(NULL, ";");
            word_count = atoi(token);                               //read wordcount

            sub_node *s = malloc(sizeof(sub_node));                 //create sub node
            if (!s)
            {
                fclose(fp);
                return FAILURE;
            }

            strcpy(s->file_name, filename);                         //initilize subnode
            s->word_count = word_count;
            s->link = NULL;

            if (m->sub_link == NULL)                                //link subnode and main node
                m->sub_link = s;
            else
            {
                sub_node *stemp = m->sub_link;
                while (stemp->link)
                    stemp = stemp->link;
                stemp->link = s;
            }
        }
    }

    fclose(fp);
    return SUCCESS;
}
