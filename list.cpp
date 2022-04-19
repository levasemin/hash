#include "hash_table.h"

struct list *list_new(const char *elem)
{
    struct list *new_list;

    new_list = (struct list *)calloc(1, sizeof(struct list));

    memcpy(new_list->elem, elem, MAX_LEN);  

    new_list->previous_list = NULL;
        
    new_list->next_list = NULL;

    new_list->state = 1;
    
    return new_list;
}

struct list *list_insert(struct list *head, const char *elem)
{
    if (head == NULL)
    {
        head = list_new(elem);
    }

    else
    {
        struct list *current_list = head; 

        while (current_list->next_list != NULL)
        {
            current_list = current_list->next_list; 
        }
        
        current_list->next_list = list_new(elem);

        current_list->next_list->previous_list = current_list;
    }

    return head;
}

struct list *list_find(struct list *head, const char *elem)
{
    if (head == NULL)
    {
        return NULL;
    }

    while (head->next_list != NULL && strncmp(head->elem, elem, MAX_LEN) != 0)
    {
        head = head->next_list; 
    }

    if (strncmp(head->elem, elem, MAX_LEN) == 0)
    {
        return head;
    }

    else
    {
        return NULL;
    }
}

struct list *list_erase(struct list *head, const char *elem, int *was)
{
    if (head == NULL)
    {

    }
    
    else if (strncmp(head->elem, elem, MAX_LEN) == 0)
    {
        *was = 1;

        head = head->next_list;

        if (head != NULL)
        {
            free(head->previous_list);
            
            head->previous_list = NULL;
        }
    }

    else
    {
        struct list *current_list = head;
        
        while (current_list->next_list != NULL && strncmp(current_list->elem, elem, MAX_LEN) != 0)
        {
            current_list = current_list->next_list; 
        }

        if (strncmp(current_list->elem, elem, MAX_LEN) == 0)
        {
            *was = 1;
            
            current_list->previous_list->next_list = current_list->next_list;
            
            if (current_list->next_list != NULL)
            {
                current_list->next_list->previous_list = current_list->previous_list;
            }

            free(current_list);
        }
    }

    return head;
}


struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
    if (where == NULL || what == NULL)
    {

    }

    else if (head == NULL)
    {
        head = what;
    }

    else
    {
        if (where->next_list != NULL)
        {
            where->next_list->previous_list = what;
        }

        what->next_list = where->next_list;
        where->next_list = what;
        what->previous_list = where;
    }

    return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
    if (where == NULL || what == NULL)
    {

    }

    else if (head == NULL)
    {
        head = what;
    }
    
    else
    {
        what->next_list = where;
        what->previous_list = where->previous_list;

        if (where != head)
        {
            where->previous_list->next_list = what;
        }

        else
        {
            head = what;
        }

        where->previous_list = what;
        
    }

    return head;
}

struct list *list_delete(struct list *head)
{
    if (head == NULL)
    {
        return NULL;
    }
    
    while (head->next_list != NULL)
    {
        head = head->next_list;
        free(head->previous_list);
    }

    free(head);

    return NULL;
}

struct list *list_next(struct list *curr)
{
    if (curr == NULL)
    {
        return NULL;
    }
    
    else
    {
        return curr->next_list;
    }
}

void list_print(struct list const *head)
{
    if (head == NULL)
    {
        printf("[]\n");
    }
    else 
    {
        printf("[");

        while (head->next_list != NULL)
        {
            printf("%s, ", head->elem);
            head = head->next_list; 
        }

        printf("%s", head->elem);
        
        printf("]\n");
    }
}
