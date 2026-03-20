#include <assert.h>
#include <stddef.h>
#include <skiresort/gondola.h>
#include <skiresort/person.h>
#include <skiresort/clock.h>


static void test_create_gondola_starts_empty(void) {
    Gondola* gondola = create_gondola();

    assert(gondola != NULL);
    assert(gondola->passenger_count == 0);
    assert(gondola->seat[0] == NULL);
    assert(gondola->seat[1] == NULL);
    assert(gondola->seat[2] == NULL);
    assert(gondola->seat[3] == NULL);
    assert(gondola->follower == NULL);

    destroy_gondola(gondola);
}


static void test_enter_gondola_fills_first_free_seat(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Gondola* gondola = create_gondola();
    Person* p1 = create_person(BUS_STOP, clock);
    Person* p2 = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(gondola != NULL);
    assert(p1 != NULL);
    assert(p2 != NULL);

    enter_gondola(gondola, p1);
    enter_gondola(gondola, p2);

    assert(gondola->passenger_count == 2);
    assert(gondola->seat[0] == p1);
    assert(gondola->seat[1] == p2);
    assert(gondola->seat[2] == NULL);
    assert(gondola->seat[3] == NULL);

    destroy_gondola(gondola);
    destroy_clock(clock);
}


static void test_enter_gondola_does_not_exceed_capacity(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Gondola* gondola = create_gondola();
    Person* p1 = create_person(BUS_STOP, clock);
    Person* p2 = create_person(BUS_STOP, clock);
    Person* p3 = create_person(BUS_STOP, clock);
    Person* p4 = create_person(BUS_STOP, clock);
    Person* p5 = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(gondola != NULL);
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(p4 != NULL);
    assert(p5 != NULL);

    enter_gondola(gondola, p1);
    enter_gondola(gondola, p2);
    enter_gondola(gondola, p3);
    enter_gondola(gondola, p4);
    enter_gondola(gondola, p5);

    assert(gondola->passenger_count == 4);
    assert(gondola->seat[0] == p1);
    assert(gondola->seat[1] == p2);
    assert(gondola->seat[2] == p3);
    assert(gondola->seat[3] == p4);

    destroy_person(p5);
    destroy_gondola(gondola);
    destroy_clock(clock);
}


static void test_exit_gondola_removes_person(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Gondola* gondola = create_gondola();
    Person* p1 = create_person(BUS_STOP, clock);
    Person* p2 = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(gondola != NULL);
    assert(p1 != NULL);
    assert(p2 != NULL);

    enter_gondola(gondola, p1);
    enter_gondola(gondola, p2);

    exit_gondola(gondola, p1);

    assert(gondola->passenger_count == 1);
    assert(gondola->seat[0] == NULL);
    assert(gondola->seat[1] == p2);

    destroy_person(p1);
    destroy_gondola(gondola);
    destroy_clock(clock);
}


int main(void) {
    test_create_gondola_starts_empty();
    test_enter_gondola_fills_first_free_seat();
    test_enter_gondola_does_not_exceed_capacity();
    test_exit_gondola_removes_person();
    return 0;
}
