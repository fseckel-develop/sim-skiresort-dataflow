#include <assert.h>
#include <stddef.h>
#include <skiresort/queue.h>
#include <skiresort/person.h>
#include <skiresort/clock.h>


static void test_create_queue_starts_empty(void) {
    Queue* queue = create_queue();

    assert(queue != NULL);
    assert(queue_is_empty(queue) == TRUE);
    assert(queue->size == 0);
    assert(queue->front == NULL);
    assert(queue->rear == NULL);

    destroy_queue(queue);
}


static void test_enqueue_adds_person(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Queue* queue = create_queue();
    Person* person = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(queue != NULL);
    assert(person != NULL);

    enqueue(queue, person);

    assert(queue_is_empty(queue) == FALSE);
    assert(queue->size == 1);
    assert(queue->front != NULL);
    assert(queue->rear != NULL);
    assert(queue->front->person == person);
    assert(queue->rear->person == person);

    destroy_queue(queue);
    destroy_clock(clock);
}


static void test_dequeue_returns_same_person(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Queue* queue = create_queue();
    Person* person = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(queue != NULL);
    assert(person != NULL);

    enqueue(queue, person);

    Person* dequeued = dequeue(queue);

    assert(dequeued == person);
    assert(queue_is_empty(queue) == TRUE);
    assert(queue->size == 0);
    assert(queue->front == NULL);
    assert(queue->rear == NULL);

    destroy_person(dequeued);
    destroy_queue(queue);
    destroy_clock(clock);
}


static void test_dequeue_on_empty_queue_returns_null(void) {
    Queue* queue = create_queue();

    assert(queue != NULL);
    assert(dequeue(queue) == NULL);

    destroy_queue(queue);
}


int main(void) {
    test_create_queue_starts_empty();
    test_enqueue_adds_person();
    test_dequeue_returns_same_person();
    test_dequeue_on_empty_queue_returns_null();
    return 0;
}
