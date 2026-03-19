#include "Queue.h"
#include <stdlib.h>


/*  create_queue():
 *  Allocates memory for a new queue and initializes its front and rear pointer.
 *      params: None
 *      return: reference to queue (or NULL)
 */
Queue* create_queue(void) {
    /* Allocating memory for a Queue struct */
    Queue* queue = malloc(sizeof(Queue));
    /* Has the memory allocation been unsuccessful? */
    if (queue) {
        /* Initializing front reference, rear reference and list size */
        queue->front = NULL;
        queue->rear = NULL;
        queue->size = 0;
    }
    return queue;
}


/*  enqueue():
 *  Adds the given person at the rear end of the given queue.
 *      params: reference to queue, reference to person
 *      return: None
 */
void enqueue(Queue* queue, Person* person) {
    /* Are the given queue reference and person reference valid? */
    if (queue && person) {
        /* Creating new node associated with the given person */
        Node* new_node = create_node(person);
        if (!new_node) {
            return;
        }
        /* Incrementing the element counter of the queue */
        queue->size++;
        /* Is the queues rear not existent? = Is the queue empty? */
        if (!queue->rear) {
            /* Setting the new node to be the queue front and rear */
            queue->front = new_node;
            queue->rear = new_node;
            return;
        }
        /* Setting the new node to be the follower of the queue rear */
        queue->rear->next = new_node;
        /* Updating the queue rear to be the newly added node */
        queue->rear = new_node;
    }
}


/*  dequeue():
 *  Removes the first person from the front of the given queue.
 *      params: reference to queue
 *      return: reference to person
 */
Person* dequeue(Queue* queue) {
    /* Is the queue not empty (and valid)? */
    if (!queue_is_empty(queue)) {
        /* Saving reference to the front node for safe queue restructuring */
        Node* dequeued_node = queue->front;
        /* Extracting attached person for return */
        Person* dequeued_person = dequeued_node->person;
        dequeued_node->person = NULL;
        /* Moving the queue front to the next node */
        queue->front = queue->front->next;
        /* Is the new queue front not a node? = Is the queue now empty? */
        if (!queue->front) {
            /* Updating the queue rear */
            queue->rear = NULL;
        }
        /* Decrementing the element counter of the queue */
        queue->size--;
        destroy_node(dequeued_node);
        return dequeued_person;
    }
    return NULL;
}


/*  queue_is_empty():
 *  Checks if the given queue has no elements.
 *      params: reference to queue
 *      return: TRUE or FALSE
 */
Boolean queue_is_empty(const Queue* queue) {
    /* Is the given queue reference valid and does it not have a front? */
    if (queue && !queue->front) {
        return TRUE;
    }
    return FALSE;
}


/*  destroy_queue():
 *  Destroys any still existing node and frees memory of the given queue.
 *      params: reference to queue
 *      return: None
 */
void destroy_queue(Queue* queue) {
    /* Is the given queue reference valid? */
    if (queue) {
        /* Preparing traversal of queue */
        Node* current = queue->front;
        while (current) {
            /* Saving reference to current node */
            Node* temp = current;
            /* Progressing queue traversal for safe node destruction */
            current = current->next;
            /* Destroying temporary saved node */
            destroy_node(temp);
        }
        /* Freeing memory of the given queue */
        free(queue);
    }
}
