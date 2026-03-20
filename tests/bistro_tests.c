#include <assert.h>
#include <skiresort/bistro.h>
#include <skiresort/station.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_welcome_customers_moves_person_into_bistro_when_open(void) {
    Clock* clock = init_clock(t(12, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(MIDDLE_STATION, lift);
    Bistro* bistro = create_bistro(station);
    Person* person = create_person(HOTEL, clock);

    person->going_to = BISTRO;
    append_list(station->people_at_station, person);

    welcome_customers(bistro);

    assert(station->people_at_station->size == 0);
    assert(bistro->customers->size == 1);

    destroy_bistro(bistro);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_welcome_customers_redirects_when_closed(void) {
    Clock* clock = init_clock(t(8, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(MIDDLE_STATION, lift);
    Bistro* bistro = create_bistro(station);
    Person* person = create_person(HOTEL, clock);

    person->going_to = BISTRO;
    append_list(station->people_at_station, person);

    welcome_customers(bistro);

    assert(station->people_at_station->size == 1);
    assert(bistro->customers->size == 0);
    assert(person->going_to != BISTRO);

    destroy_bistro(bistro);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_customers_are_eating_moves_finished_customer_back_to_station(void) {
    Clock* clock = init_clock(t(12, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(MIDDLE_STATION, lift);
    Bistro* bistro = create_bistro(station);
    Person* person = create_person(HOTEL, clock);

    person->activity_duration = t(0, 0, 10);
    append_list(bistro->customers, person);

    customers_are_eating(bistro);

    assert(bistro->customers->size == 0);
    assert(station->people_at_station->size == 1);

    destroy_bistro(bistro);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


int main(void) {
    test_welcome_customers_moves_person_into_bistro_when_open();
    test_welcome_customers_redirects_when_closed();
    test_customers_are_eating_moves_finished_customer_back_to_station();
    return 0;
}
