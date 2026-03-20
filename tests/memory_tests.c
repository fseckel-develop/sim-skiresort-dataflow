#include <assert.h>
#include <stddef.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>
#include <skiresort/queue.h>
#include <skiresort/list.h>
#include <skiresort/gondola.h>
#define NUM_SAMPLES 100


static void test_repeated_create_destroy_clock(void) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Clock* clock = init_clock(t(8, 40, 0));
        assert(clock != NULL);
        destroy_clock(clock);
    }
}


static void test_repeated_create_destroy_person(void) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Clock* clock = init_clock(t(8, 40, 0));
        Person* person = create_person(BUS_STOP, clock);

        assert(clock != NULL);
        assert(person != NULL);

        destroy_person(person);
        destroy_clock(clock);
    }
}


static void test_repeated_queue_lifecycle(void) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Clock* clock = init_clock(t(8, 40, 0));
        Queue* queue = create_queue();
        Person* person = create_person(BUS_STOP, clock);

        assert(clock != NULL);
        assert(queue != NULL);
        assert(person != NULL);

        enqueue(queue, person);
        assert(queue->size == 1);

        const Person* dequeued = dequeue(queue);
        assert(dequeued == person);

        destroy_person(person);
        destroy_queue(queue);
        destroy_clock(clock);
    }
}


static void test_repeated_list_lifecycle(void) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Clock* clock = init_clock(t(8, 40, 0));
        List* list = create_list();
        Person* person = create_person(BUS_STOP, clock);

        assert(clock != NULL);
        assert(list != NULL);
        assert(person != NULL);

        append_list(list, person);
        remove_person(list, person);

        destroy_person(person);
        destroy_list(list);
        destroy_clock(clock);
    }
}


static void test_repeated_gondola_lifecycle(void) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Clock* clock = init_clock(t(8, 40, 0));
        Gondola* gondola = create_gondola();
        Person* person = create_person(BUS_STOP, clock);

        assert(clock != NULL);
        assert(gondola != NULL);
        assert(person != NULL);

        enter_gondola(gondola, person);
        exit_gondola(gondola, person);

        destroy_person(person);
        destroy_gondola(gondola);
        destroy_clock(clock);
    }
}


int main(void) {
    test_repeated_create_destroy_clock();
    test_repeated_create_destroy_person();
    test_repeated_queue_lifecycle();
    test_repeated_list_lifecycle();
    test_repeated_gondola_lifecycle();
    return 0;
}
