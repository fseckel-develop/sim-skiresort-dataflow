#include "Node.h"
#include <stdlib.h>


/*  create_node():
 *  Allocates memory for a new node and associates it with the given person.
 *      params: reference to person
 *      return: reference to node (or NULL)
 */
Node* create_node(Person* person) {
    if (person) {
        Node* node = calloc(1, sizeof(Node));
        if (node) {
            /* Associating the given person with the new node */
            node->person = person;
            return node;
        }
    }
    return NULL;
}


/*  destroy_node():
 *  Destroys associated person and frees memory of the given node.
 *      params: reference to node
 *      return: none
 */
void destroy_node(Node* node) {
    /* Is the given node reference valid? */
    if (node) {
        /* Destroying associated person */
        destroy_person(node->person);
        /* Freeing memory of the given node */
        free(node);
    }
}
