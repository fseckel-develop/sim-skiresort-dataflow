#include <assert.h>
#include <stddef.h>
#include <skiresort/person.h>
#include <skiresort/clock.h>


static void test_activity_finished_zero(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Person* person = create_person(HOTEL, clock);

    assert(person != NULL);
    person->activity_duration = t(0, 0, 0);

    assert(activity_finished(person) == TRUE);

    destroy_person(person);
    destroy_clock(clock);
}


static void test_proceed_activity(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Person* person = create_person(HOTEL, clock);

    assert(person != NULL);
    person->activity_duration = t(0, 1, 0);

    proceed_activity(person);
    assert(person->activity_duration.min == 0);
    assert(person->activity_duration.s == 50);

    destroy_person(person);
    destroy_clock(clock);
}


static void test_arrival_density_boundaries(void) {
    assert(arrival_density(t(8, 40, 0)) == 0.0);
    assert(arrival_density(t(11, 0, 0)) == 1.0);
    assert(arrival_density(t(13, 0, 0)) == 1.0);
    assert(arrival_density(t(15,30, 0)) == 0.5);
    assert(arrival_density(t(17, 0, 0)) == 0.0);
}


int main(void) {
    test_activity_finished_zero();
    test_proceed_activity();
    test_arrival_density_boundaries();
    return 0;
}
