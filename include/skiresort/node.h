#ifndef NODE_H
#define NODE_H
#include <skiresort/person.h>


/*  Attributes of Node:
 *   - reference to associated person
 *   - reference to previous node
 *   - reference to next node
 */
typedef struct Node {
    Person* person;
    struct Node* prev;
    struct Node* next;
} Node;


Node* create_node(Person*);
void destroy_node(Node*);


#endif /* NODE_H */
