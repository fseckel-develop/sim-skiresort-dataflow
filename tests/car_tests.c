#include <assert.h>
#include <stddef.h>
#include <skiresort/car.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_create_car_initializes_valid_state(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Car* car = create_car(clock);

    assert(clock != NULL);
    assert(car != NULL);

    assert(1 <= car->passenger_count);
    assert(car->passenger_count <= 5);
    assert(car->passengers != NULL);
    assert(car->passengers->size == car->passenger_count);

    destroy_car(car);
    destroy_clock(clock);
}


static void test_car_passengers_get_same_car_id(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Car* car = create_car(clock);
    Node* current;

    assert(clock != NULL);
    assert(car != NULL);

    current = car->passengers->front;
    while (current) {
        assert(current->person != NULL);
        assert(current->person->car_id == car->id);
        current = current->next;
    }

    destroy_car(car);
    destroy_clock(clock);
}


static void test_leave_car_reduces_passenger_count_in_list(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Car* car = create_car(clock);
    const int initial_size = car->passengers->size;

    assert(clock != NULL);
    assert(car != NULL);
    assert(initial_size > 0);

    Person* person = leave_car(car);

    assert(person != NULL);
    assert(car->passengers->size == initial_size - 1);

    destroy_person(person);
    destroy_car(car);
    destroy_clock(clock);
}


static void test_leave_car_until_empty(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Car* car = create_car(clock);

    assert(clock != NULL);
    assert(car != NULL);

    while (!car_is_empty(car)) {
        Person* person = leave_car(car);
        assert(person != NULL);
        destroy_person(person);
    }

    assert(car_is_empty(car) == TRUE);
    assert(leave_car(car) == NULL);

    destroy_car(car);
    destroy_clock(clock);
}


static void test_car_is_full_after_creation(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Car* car = create_car(clock);

    assert(clock != NULL);
    assert(car != NULL);

    assert(car_is_full(car) == TRUE);

    destroy_car(car);
    destroy_clock(clock);
}


static void test_car_is_not_full_after_person_leaves(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Car* car = create_car(clock);

    assert(clock != NULL);
    assert(car != NULL);

    Person* person = leave_car(car);
    assert(person != NULL);

    assert(car_is_full(car) == FALSE);

    destroy_person(person);
    destroy_car(car);
    destroy_clock(clock);
}


int main(void) {
    test_create_car_initializes_valid_state();
    test_car_passengers_get_same_car_id();
    test_leave_car_reduces_passenger_count_in_list();
    test_leave_car_until_empty();
    test_car_is_full_after_creation();
    test_car_is_not_full_after_person_leaves();
    return 0;
}
