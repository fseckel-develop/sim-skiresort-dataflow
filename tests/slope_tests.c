#include <assert.h>
#include <skiresort/slope.h>
#include <skiresort/station.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_let_people_enter_moves_matching_person_to_slope(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* start = create_station(MIDDLE_STATION, lift);
    Station* finish = create_station(VALLEY_STATION, lift);
    Slope* slope = create_slope(SLOPE_B1, start, finish, 3, 11);
    Person* person = create_person(BUS_STOP, clock);

    person->going_to = SLOPE_B1;
    append_list(start->people_at_station, person);

    let_people_enter(slope);

    assert(start->people_at_station->size == 0);
    assert(slope->skiers->size == 1);
    assert(slope->total_entries == 1);

    destroy_slope(slope);
    destroy_station(start);
    destroy_station(finish);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_people_are_skiing_moves_finished_skier_to_finish_station(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* start = create_station(MIDDLE_STATION, lift);
    Station* finish = create_station(VALLEY_STATION, lift);
    Slope* slope = create_slope(SLOPE_B1, start, finish, 3, 11);
    Person* person = create_person(BUS_STOP, clock);

    person->activity_duration = t(0, 0, 10);
    append_list(slope->skiers, person);

    people_are_skiing(slope);

    assert(slope->skiers->size == 0);
    assert(finish->people_at_station->size == 1);

    destroy_slope(slope);
    destroy_station(start);
    destroy_station(finish);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_collect_skiers_with_snowmobile_moves_all_to_dropoff(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* start = create_station(SUMMIT_STATION, lift);
    Station* finish = create_station(VALLEY_STATION, lift);
    Station* drop_off = create_station(VALLEY_STATION, lift);
    Slope* slope = create_slope(SLOPE_S1, start, finish, 9, 34);
    Person* p1 = create_person(BUS_STOP, clock);
    Person* p2 = create_person(HOTEL, clock);

    append_list(slope->skiers, p1);
    append_list(slope->skiers, p2);

    collect_skiers_with_snowmobile(slope, drop_off);

    assert(slope->skiers->size == 0);
    assert(drop_off->people_at_station->size == 2);

    destroy_slope(slope);
    destroy_station(start);
    destroy_station(finish);
    destroy_station(drop_off);
    destroy_lift(lift);
    destroy_clock(clock);
}


int main(void) {
    test_let_people_enter_moves_matching_person_to_slope();
    test_people_are_skiing_moves_finished_skier_to_finish_station();
    test_collect_skiers_with_snowmobile_moves_all_to_dropoff();
    return 0;
}
