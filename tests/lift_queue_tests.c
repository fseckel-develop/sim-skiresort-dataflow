#include <assert.h>
#include <skiresort/lift_queue.h>
#include <skiresort/station.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>
#include <skiresort/person.h>


static void test_set_entry_gondola_maps_correctly(void) {
    Clock* clock = init_clock(t(9, 0, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    Station* middle = create_station(MIDDLE_STATION, lift);
    Station* summit = create_station(SUMMIT_STATION, lift);

    LiftQueue* valley_queue = create_lift_queue(VALLEY_LIFT_QUEUE, valley);
    LiftQueue* middle_up_queue = create_lift_queue(MIDDLE_LIFT_QUEUE_UP, middle);
    LiftQueue* middle_down_queue = create_lift_queue(MIDDLE_LIFT_QUEUE_DOWN, middle);
    LiftQueue* summit_queue = create_lift_queue(SUMMIT_LIFT_QUEUE, summit);

    assert(valley_queue->entry_gondola == valley->upward_gondola);
    assert(middle_up_queue->entry_gondola == middle->upward_gondola);
    assert(middle_down_queue->entry_gondola == middle->downward_gondola);
    assert(summit_queue->entry_gondola == summit->downward_gondola);

    destroy_lift_queue(valley_queue);
    destroy_lift_queue(middle_up_queue);
    destroy_lift_queue(middle_down_queue);
    destroy_lift_queue(summit_queue);
    destroy_station(valley);
    destroy_station(middle);
    destroy_station(summit);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_ticket_control_first_purchase_sets_valid_ticket(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    LiftQueue* queue = create_lift_queue(VALLEY_LIFT_QUEUE, valley);
    Person* person = create_person(BUS_STOP, clock);

    person->paid_entries = NO_FIRST_PURCHASE;
    ticket_control(queue, person);

    assert(person->paid_entries == TEN_TICKET || person->paid_entries == DAY_TICKET);
    assert(queue->sold_ten_tickets + queue->sold_day_tickets == 1);

    destroy_person(person);
    destroy_lift_queue(queue);
    destroy_station(valley);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_ticket_control_decrements_ten_ticket(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    LiftQueue* queue = create_lift_queue(VALLEY_LIFT_QUEUE, valley);
    Person* person = create_person(BUS_STOP, clock);

    person->paid_entries = 5;
    ticket_control(queue, person);

    assert(person->paid_entries == 4);

    destroy_person(person);
    destroy_lift_queue(queue);
    destroy_station(valley);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_ticket_control_rebuys_when_ten_ticket_is_empty(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Lift* lift = create_lift(clock);
    Station* valley = create_station(VALLEY_STATION, lift);
    LiftQueue* queue = create_lift_queue(VALLEY_LIFT_QUEUE, valley);
    Person* person = create_person(BUS_STOP, clock);

    person->paid_entries = 0;
    ticket_control(queue, person);

    assert(person->paid_entries == TEN_TICKET);
    assert(queue->sold_ten_tickets == 1);

    destroy_person(person);
    destroy_lift_queue(queue);
    destroy_station(valley);
    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_choose_exit_station_for_forced_routes(void) {
    Clock* clock = init_clock(t(10, 0, 0));
    Person* person_up = create_person(BUS_STOP, clock);
    Person* person_down = create_person(BUS_STOP, clock);

    person_up->going_to = MIDDLE_LIFT_QUEUE_UP;
    choose_exit_station(person_up);
    assert(person_up->going_to == SUMMIT_STATION);

    person_down->going_to = MIDDLE_LIFT_QUEUE_DOWN;
    choose_exit_station(person_down);
    assert(person_down->going_to == VALLEY_STATION);

    destroy_person(person_up);
    destroy_person(person_down);
    destroy_clock(clock);
}


int main(void) {
    test_set_entry_gondola_maps_correctly();
    test_ticket_control_first_purchase_sets_valid_ticket();
    test_ticket_control_decrements_ten_ticket();
    test_ticket_control_rebuys_when_ten_ticket_is_empty();
    test_choose_exit_station_for_forced_routes();
    return 0;
}
