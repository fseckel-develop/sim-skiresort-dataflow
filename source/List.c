#include "List.h"
#include <stdlib.h>


/*  create_list():
 *  Allocates memory for a new list and initializes its front pointer.
 *      params: none
 *      return: reference to list (or NULL)
 */
List* create_list(void) {
    /* Allocating memory for a List struct */
    List* list = calloc(1, sizeof(List));
    return list;
}


/*  append_list():
 *  Adds the given person at the front of the given list ensuring double connection.
 *      params: reference to list, reference to person
 *      return: none
 */
void append_list(List* list, Person* person) {
    /* Are the given list reference and person reference valid? */
    if (list && person) {
        /* Creating new node associated with the given person */
        Node* new_node = create_node(person);
        if (!new_node) {
            return;
        }
        /* Is the list front already existing? */
        if (list->front) {
            /* Setting the next of the new node to be the list front */
            new_node->next = list->front;
            /* Setting the previous node of the front node to be the new node */
            list->front->prev = new_node;
        }
        /* Updating the list front to be the newly added node */
        list->front = new_node;
        /* Incrementing the element counter of the list */
        list->size++;
    }
}


/*  remove_person():
 *  Removes the given person from the given list, maintaining double connection.
 *      params: reference to list, reference to person
 *      return: none
 */
void remove_person(List* list, const Person* person) {
    /* Is the list not empty (and valid) and is the given person reference valid? */
    if (!list_is_empty(list) && person) {
        /* Preparing traversal of list */
        Node* current = list->front;
        while (current) {
            /* Is the given person equal to the person associated with current node? */
            if (current->person == person) {
                /* Is the current node at the front of the list? */
                if (current == list->front) {
                    /* Updating list front to be the next node */
                    list->front = current->next;
                }
                else {
                    /* Setting the next of the previous node to be the next of the current node */
                    current->prev->next = current->next;
                }
                /* Is the current node not at the rear of the list? */
                if (current->next) {
                    /* Setting the previous of the next node to be the previous of the current node */
                    current->next->prev = current->prev;
                }
                /* Breaking association with person to protect it from node destruction */
                current->person = NULL;
                /* Destroying the current node after disconnecting it from neighboring nodes */
                destroy_node(current);
                /* Decrementing the element counter of the list */
                list->size--;
                return;
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  list_is_empty():
 *  Checks if the given list has no elements.
 *      params: reference to list
 *      return: TRUE or FALSE
 */
Boolean list_is_empty(const List* list) {
    /* Is the given list reference valid and is the front reference not set? */
    if (list && !list->front) {
        return TRUE;
    }
    return FALSE;
}


/*  destroy_list():
 *  Destroys any still existing node and frees memory of the given list.
 *      params: reference to list
 *      return: none
 */
void destroy_list(List* list) {
    /* Is the given list reference valid? */
    if (list) {
        /* Preparing traversal of list */
        Node* current = list->front;
        while (current) {
            /* Saving reference to current node */
            Node* temp = current;
            /* Progressing list traversal for safe node destruction */
            current = current->next;
            /* Destroying temporary saved node */
            destroy_node(temp);
        }
        /* Freeing memory of the given list */
        free(list);
    }
}
