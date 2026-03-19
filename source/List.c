#include "List.h"
#include <stdlib.h>


/*  create_list():
 *  Allocates memory for a new list and initializes its front pointer.
 *      params: none
 *      return: reference to list (or NULL)
 */
List* create_list(void) {
    List* list = calloc(1, sizeof(List));
    return list;
}


/*  append_list():
 *  Adds the given person at the front of the given list ensuring double connection.
 *      params: reference to list, reference to person
 *      return: none
 */
void append_list(List* list, Person* person) {
    if (list && person) {
        Node* new_node = create_node(person);
        if (!new_node) {
            return;
        }
        if (list->front) {
            new_node->next = list->front;
            list->front->prev = new_node;
        }
        list->front = new_node;
        list->size++;
    }
}


/*  remove_person():
 *  Removes the given person from the given list, maintaining double connection.
 *      params: reference to list, reference to person
 *      return: none
 */
void remove_person(List* list, const Person* person) {
    if (!list_is_empty(list) && person) {
        Node* current = list->front;
        while (current) {
            if (current->person == person) {
                /* Is the current node at the front of the list? */
                if (current == list->front) {
                    list->front = current->next;
                }
                else {
                    current->prev->next = current->next;
                }
                /* Is the current node not at the rear of the list? */
                if (current->next) {
                    current->next->prev = current->prev;
                }
                current->person = NULL;
                destroy_node(current);
                list->size--;
                return;
            }
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
    if (list) {
        Node* current = list->front;
        while (current) {
            Node* temp = current;
            current = current->next;
            destroy_node(temp);
        }
        free(list);
    }
}
