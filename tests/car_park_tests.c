#include <assert.h>
#include <stddef.h>
#include <skiresort/car_park.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_plan_departure_time_sets_same_time_for_all_passengers(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Car* car = create_car(clock);

    assert(car != NULL);
    assert(car->passengers != NULL);
    assert(car->passengers->front != NULL);

    plan_departure_time(car);
    const Time expected = car->passengers->front->person->departure_time;

    const Node* current = car->passengers->front;
    while (current) {
        assert(s(current->person->departure_time) == s(expected));
        current = current->next;
    }

    destroy_car(car);
    destroy_clock(clock);
}


static void test_enter_own_car_puts_person_into_matching_car(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(VALLEY_STATION, lift);
    CarPark* car_park = create_car_park(station, clock);
    Car* car = create_car(clock);
    Person* person = create_person(CAR_PARK, clock);

    car_park->parked_cars[0] = car;
    car_park->car_count = 1;

    person->car_id = car->id;

    enter_own_car(car_park, person);

    assert(car->passengers->front != NULL);

    destroy_car_park(car_park);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


int main(void) {
    test_plan_departure_time_sets_same_time_for_all_passengers();
    test_enter_own_car_puts_person_into_matching_car();
    return 0;
}
