#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <skiresort/bus.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_init_passenger_count_is_zero_when_no_arrivals_expected(void) {
    assert(init_passenger_count(t(8, 40, 0)) == 0);
    assert(init_passenger_count(t(18, 0, 0)) == 0);
}


static void test_init_passenger_count_stays_within_bus_capacity(void) {
    for (int i = 0; i < 1000; i++) {
        const int count = init_passenger_count(t(11, 0, 0));
        assert(0 <= count);
        assert(count <= 50);
    }
}


static void test_create_bus_initializes_valid_state(void) {
    Clock* clock = init_clock(t(11, 0, 0));
    Bus* bus = create_bus(clock);

    assert(clock != NULL);
    assert(bus != NULL);
    assert(bus->passengers != NULL);
    assert(0 <= bus->passenger_count);
    assert(bus->passenger_count <= 50);
    assert(bus->passengers->size == bus->passenger_count);

    destroy_bus(bus);
    destroy_clock(clock);
}


static void test_leave_bus_reduces_passenger_count_in_list(void) {
    Clock* clock = init_clock(t(11, 0, 0));
    Bus* bus = create_bus(clock);

    assert(clock != NULL);
    assert(bus != NULL);

    if (!bus_is_empty(bus)) {
        const int initial_size = bus->passengers->size;
        Person* person = leave_bus(bus);

        assert(person != NULL);
        assert(bus->passengers->size == initial_size - 1);

        destroy_person(person);
    }

    destroy_bus(bus);
    destroy_clock(clock);
}


static void test_leave_bus_until_empty(void) {
    Clock* clock = init_clock(t(11, 0, 0));
    Bus* bus = create_bus(clock);

    assert(clock != NULL);
    assert(bus != NULL);

    while (!bus_is_empty(bus)) {
        Person* person = leave_bus(bus);
        assert(person != NULL);
        destroy_person(person);
    }

    assert(bus_is_empty(bus) == TRUE);
    assert(leave_bus(bus) == NULL);

    destroy_bus(bus);
    destroy_clock(clock);
}


static void test_bus_is_full_when_manually_filled_to_capacity(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Bus* bus = calloc(1, sizeof(Bus));

    assert(clock != NULL);
    assert(bus != NULL);

    bus->passengers = create_list();
    bus->passenger_count = 50;

    assert(bus->passengers != NULL);

    for (int i = 0; i < 50; i++) {
        Person* person = create_person(BUS_STOP, clock);
        assert(person != NULL);
        append_list(bus->passengers, person);
    }

    assert(bus->passengers->size == 50);
    assert(bus_is_full(bus) == TRUE);

    destroy_bus(bus);
    destroy_clock(clock);
}


static void test_bus_is_not_full_when_below_capacity(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Bus* bus = calloc(1, sizeof(Bus));

    assert(clock != NULL);
    assert(bus != NULL);

    bus->passengers = create_list();
    bus->passenger_count = 50;

    assert(bus->passengers != NULL);

    for (int i = 0; i < 49; i++) {
        Person* person = create_person(BUS_STOP, clock);
        assert(person != NULL);
        append_list(bus->passengers, person);
    }

    assert(bus->passengers->size == 49);
    assert(bus_is_full(bus) == FALSE);

    destroy_bus(bus);
    destroy_clock(clock);
}


int main(void) {
    test_init_passenger_count_is_zero_when_no_arrivals_expected();
    test_init_passenger_count_stays_within_bus_capacity();
    test_create_bus_initializes_valid_state();
    test_leave_bus_reduces_passenger_count_in_list();
    test_leave_bus_until_empty();
    test_bus_is_full_when_manually_filled_to_capacity();
    test_bus_is_not_full_when_below_capacity();
    return 0;
}
