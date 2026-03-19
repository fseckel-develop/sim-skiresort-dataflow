#include <skiresort/queue.h>
#include <stdlib.h>


/*  create_queue():
 *  Allocates memory for a new queue and initializes its front and rear pointer.
 *      params: None
 *      return: reference to queue (or NULL)
 */
Queue* create_queue(void) {
    Queue* queue = calloc(1, sizeof(Queue));
    return queue;
}


/*  enqueue():
 *  Adds the given person at the rear end of the given queue.
 *      params: reference to queue, reference to person
 *      return: None
 */
void enqueue(Queue* queue, Person* person) {
    if (queue && person) {
        Node* new_node = create_node(person);
        if (!new_node) {
            return;
        }
        queue->size++;
        if (!queue->rear) {
            queue->front = new_node;
            queue->rear = new_node;
            return;
        }
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}


/*  dequeue():
 *  Removes the first person from the front of the given queue.
 *      params: reference to queue
 *      return: reference to person
 */
Person* dequeue(Queue* queue) {
    if (!queue_is_empty(queue)) {
        Node* dequeued_node = queue->front;
        Person* dequeued_person = dequeued_node->person;
        dequeued_node->person = NULL;
        queue->front = queue->front->next;
        /* Is the new queue front not a node? = Is the queue now empty? */
        if (!queue->front) {
            queue->rear = NULL;
        }
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
    if (queue) {
        Node* current = queue->front;
        while (current) {
            Node* temp = current;
            current = current->next;
            destroy_node(temp);
        }
        free(queue);
    }
}
