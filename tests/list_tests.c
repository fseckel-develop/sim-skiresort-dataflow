#include <assert.h>
#include <stddef.h>
#include <skiresort/list.h>
#include <skiresort/person.h>
#include <skiresort/clock.h>


static void test_create_list_starts_empty(void) {
    List* list = create_list();

    assert(list != NULL);
    assert(list_is_empty(list) == TRUE);
    assert(list->size == 0);
    assert(list->front == NULL);

    destroy_list(list);
}


static void test_append_list_adds_person_at_front(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    List* list = create_list();
    Person* person = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(list != NULL);
    assert(person != NULL);

    append_list(list, person);

    assert(list_is_empty(list) == FALSE);
    assert(list->size == 1);
    assert(list->front != NULL);
    assert(list->front->person == person);
    assert(list->front->prev == NULL);
    assert(list->front->next == NULL);

    destroy_list(list);
    destroy_clock(clock);
}


static void test_remove_person_from_single_element_list(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    List* list = create_list();
    Person* person = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(list != NULL);
    assert(person != NULL);

    append_list(list, person);
    remove_person(list, person);

    assert(list_is_empty(list) == TRUE);
    assert(list->size == 0);
    assert(list->front == NULL);

    destroy_person(person);
    destroy_list(list);
    destroy_clock(clock);
}


static void test_remove_front_from_multi_element_list(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    List* list = create_list();
    Person* first = create_person(BUS_STOP, clock);
    Person* second = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(list != NULL);
    assert(first != NULL);
    assert(second != NULL);

    append_list(list, first);
    append_list(list, second);

    assert(list->front->person == second);
    assert(list->size == 2);

    remove_person(list, second);

    assert(list->size == 1);
    assert(list->front != NULL);
    assert(list->front->person == first);
    assert(list->front->prev == NULL);
    assert(list->front->next == NULL);

    destroy_person(second);
    destroy_list(list);
    destroy_clock(clock);
}


int main(void) {
    test_create_list_starts_empty();
    test_append_list_adds_person_at_front();
    test_remove_person_from_single_element_list();
    test_remove_front_from_multi_element_list();
    return 0;
}
