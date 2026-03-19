#ifndef QUEUE_H
#define QUEUE_H
#include <skiresort/node.h>


/*  Attributes of Queue:
 *   - reference to front node
 *   - reference to rear node
 *   - number of queue elements
 */
typedef struct Queue {
    Node* front;
    Node* rear;
    int size;
} Queue;


Queue* create_queue(void);
void enqueue(Queue*, Person*);
Person* dequeue(Queue*);
Boolean queue_is_empty(const Queue*);
void destroy_queue(Queue*);


#endif /* QUEUE_H */
