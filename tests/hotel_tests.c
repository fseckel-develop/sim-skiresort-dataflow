#include <assert.h>
#include <skiresort/hotel.h>
#include <skiresort/station.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_return_to_hotel_moves_person_from_station_to_hotel(void) {
    Clock* clock = init_clock(t(16, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(VALLEY_STATION, lift);
    Hotel* hotel = create_hotel(station, clock);
    Person* person = create_person(HOTEL, clock);

    person->going_to = HOTEL;
    append_list(station->people_at_station, person);

    return_to_hotel(hotel);

    assert(station->people_at_station->size == 0);
    assert(hotel->present_guests->size >= 1);

    destroy_hotel(hotel);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_guests_are_resting_progresses_activity(void) {
    Clock* clock = init_clock(t(16, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(VALLEY_STATION, lift);
    Hotel* hotel = create_hotel(station, clock);
    Person* person = create_person(HOTEL, clock);

    person->activity_duration = t(0, 1, 0);
    append_list(hotel->present_guests, person);

    guests_are_resting(hotel);

    assert(person->activity_duration.min == 0);
    assert(person->activity_duration.s == 50);

    destroy_hotel(hotel);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


int main(void) {
    test_return_to_hotel_moves_person_from_station_to_hotel();
    test_guests_are_resting_progresses_activity();
    return 0;
}
