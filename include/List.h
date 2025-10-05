#ifndef LIST_H
#define LIST_H
#include "Node.h"


/*  Attributes of List:
 *   - reference to first node
 *   - number of list elements
 */
typedef struct List {
    Node* front;
    int size;
} List;


List* create_list(void);
void append_list(List*, Person*);
void remove_person(List*, const Person*);
Boolean list_is_empty(const List*);
void destroy_list(List*);


#endif /* LIST_H */
