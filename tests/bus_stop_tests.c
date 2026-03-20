#include <assert.h>
#include <skiresort/bus_stop.h>
#include <skiresort/station.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_plan_on_last_bus_clamps_late_departure(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Person* person = create_person(BUS_STOP, clock);

    person->departure_time = t(20, 0, 0);
    plan_on_last_bus(person);

    assert(s(person->departure_time) == s(t(19, 30, 0)));

    destroy_person(person);
    destroy_clock(clock);
}


static void test_return_to_bus_stop_moves_person_from_station_to_queue(void) {
    Clock* clock = init_clock(t(18, 0, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(VALLEY_STATION, lift);
    BusStop* bus_stop = create_bus_stop(station, clock);
    Person* person = create_person(BUS_STOP, clock);

    person->going_to = BUS_STOP;
    append_list(station->people_at_station, person);

    return_to_bus_stop(bus_stop);

    assert(station->people_at_station->size == 0);
    assert(bus_stop->queue->size == 1);

    destroy_bus_stop(bus_stop);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_get_taxi_for_late_bus_passengers_clears_queue_after_last_bus(void) {
    Clock* clock = init_clock(t(20, 14, 0));
    Lift* lift = create_lift(clock);
    Station* station = create_station(VALLEY_STATION, lift);
    BusStop* bus_stop = create_bus_stop(station, clock);
    Person* p1 = create_person(BUS_STOP, clock);
    Person* p2 = create_person(BUS_STOP, clock);

    enqueue(bus_stop->queue, p1);
    enqueue(bus_stop->queue, p2);

    get_taxi_for_late_bus_passengers(bus_stop);

    assert(bus_stop->queue->size == 0);
    assert(queue_is_empty(bus_stop->queue) == TRUE);

    destroy_bus_stop(bus_stop);
    destroy_station(station);
    destroy_lift(lift);
    destroy_clock(clock);
}


int main(void) {
    test_plan_on_last_bus_clamps_late_departure();
    test_return_to_bus_stop_moves_person_from_station_to_queue();
    test_get_taxi_for_late_bus_passengers_clears_queue_after_last_bus();
    return 0;
}
