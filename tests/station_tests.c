#include <assert.h>
#include <stddef.h>
#include <skiresort/station.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_create_station_sets_correct_id_and_list(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(VALLEY_STATION, lift);

    assert(clock != NULL);
    assert(lift != NULL);
    assert(station != NULL);

    assert(station->id == VALLEY_STATION);
    assert(station->people_at_station != NULL);
    assert(station->direct_person != NULL);

    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_set_gondolas_assigns_correct_lift_positions(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    Station* middle = create_station(MIDDLE_STATION, lift);
    Station* summit = create_station(SUMMIT_STATION, lift);

    assert(clock != NULL);
    assert(lift != NULL);
    assert(valley != NULL);
    assert(middle != NULL);
    assert(summit != NULL);

    set_gondolas(valley, lift);
    set_gondolas(middle, lift);
    set_gondolas(summit, lift);

    assert(valley->upward_gondola == lift->valley_entry);
    assert(valley->downward_gondola == lift->valley_exit);

    assert(middle->upward_gondola == lift->middle_upward);
    assert(middle->downward_gondola == lift->middle_downward);

    assert(summit->upward_gondola == lift->summit_exit);
    assert(summit->downward_gondola == lift->summit_entry);

    destroy_station(valley);
    destroy_station(middle);
    destroy_station(summit);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_go_to_station_adds_person_and_directs_from_valley(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    Person* person = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(lift != NULL);
    assert(valley != NULL);
    assert(person != NULL);

    person->departure_time = t(12, 0, 0);
    person->going_to = UNDEFINED;

    go_to_station(valley, person);

    assert(valley->people_at_station->size == 1);
    assert(valley->people_at_station->front != NULL);
    assert(valley->people_at_station->front->person == person);
    assert(person->going_to == VALLEY_LIFT_QUEUE);

    destroy_station(valley);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_go_to_station_directs_to_origin_after_departure_time(void) {
    Clock* clock = init_clock(t(18, 0, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    Person* person = create_person(BUS_STOP, clock);

    assert(clock != NULL);
    assert(lift != NULL);
    assert(valley != NULL);
    assert(person != NULL);

    person->departure_time = t(17, 0, 0);
    person->going_to = UNDEFINED;

    go_to_station(valley, person);

    assert(person->going_to == BUS_STOP);

    destroy_station(valley);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_direct_people_at_middle_station_sets_valid_destination(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Person* person = create_person(HOTEL, clock);

    assert(clock != NULL);
    assert(person != NULL);

    person->skill_level = BEGINNER;
    person->going_to = UNDEFINED;

    direct_people_at_middle_station(person);

    assert(
        person->going_to == BISTRO ||
        person->going_to == MIDDLE_LIFT_QUEUE_UP ||
        person->going_to == MIDDLE_LIFT_QUEUE_DOWN ||
        person->going_to == SLOPE_B1 ||
        person->going_to == SLOPE_R1
    );

    destroy_person(person);
    destroy_clock(clock);
}


static void test_direct_people_at_summit_station_sets_valid_destination(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Person* person = create_person(HOTEL, clock);

    assert(clock != NULL);
    assert(person != NULL);

    person->skill_level = ADVANCED;
    person->going_to = UNDEFINED;

    direct_people_at_summit_station(person);

    assert(
        person->going_to == SLOPE_B2 ||
        person->going_to == SLOPE_R2 ||
        person->going_to == SLOPE_S1
    );

    destroy_person(person);
    destroy_clock(clock);
}


int main(void) {
    test_create_station_sets_correct_id_and_list();
    test_set_gondolas_assigns_correct_lift_positions();
    test_go_to_station_adds_person_and_directs_from_valley();
    test_go_to_station_directs_to_origin_after_departure_time();
    test_direct_people_at_middle_station_sets_valid_destination();
    test_direct_people_at_summit_station_sets_valid_destination();
    return 0;
}
