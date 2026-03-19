#include <skiresort/node.h>
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
    if (node) {
        destroy_person(node->person);
        free(node);
    }
}
