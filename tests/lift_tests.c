#include <assert.h>
#include <stddef.h>
#include <skiresort/lift.h>
#include <skiresort/clock.h>


static int distance_from_start_to_target(const Gondola* start, const Gondola* target) {
    const Gondola* current = start;
    for (int i = 0; i < TOTAL_GONDOLAS; i++) {
        if (current == target) {
            return i;
        }
        current = current->follower;
    }
    return -1;
}


static void test_create_lift_initializes_station_gondolas(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Lift* lift = create_lift(clock);

    assert(clock != NULL);
    assert(lift != NULL);

    assert(lift->valley_entry != NULL);
    assert(lift->valley_exit != NULL);
    assert(lift->middle_upward != NULL);
    assert(lift->middle_downward != NULL);
    assert(lift->summit_entry != NULL);
    assert(lift->summit_exit != NULL);

    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_lift_gondola_chain_is_circular(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Lift* lift = create_lift(clock);

    assert(clock != NULL);
    assert(lift != NULL);
    assert(lift->valley_exit != NULL);

    const Gondola* current = lift->valley_exit;
    for (int i = 0; i < TOTAL_GONDOLAS; i++) {
        assert(current != NULL);
        current = current->follower;
    }

    assert(current == lift->valley_exit);

    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_init_station_gondolas_sets_expected_offsets(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    Lift* lift = create_lift(clock);

    assert(clock != NULL);
    assert(lift != NULL);

    assert(distance_from_start_to_target(lift->valley_exit, lift->middle_downward) == 24);
    assert(distance_from_start_to_target(lift->valley_exit, lift->summit_entry) == 54);
    assert(distance_from_start_to_target(lift->valley_exit, lift->summit_exit) == 55);
    assert(distance_from_start_to_target(lift->valley_exit, lift->middle_upward) == 85);

    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_move_lift_advances_station_gondolas_during_operating_hours(void) {
    Clock* clock = init_clock(t(9, 0, 0));
    Lift* lift = create_lift(clock);

    assert(clock != NULL);
    assert(lift != NULL);

    const Gondola* old_valley_entry = lift->valley_entry;
    const Gondola* old_valley_exit = lift->valley_exit;
    const Gondola* old_middle_upward = lift->middle_upward;
    const Gondola* old_middle_downward = lift->middle_downward;
    const Gondola* old_summit_entry = lift->summit_entry;
    const Gondola* old_summit_exit = lift->summit_exit;

    move_lift(lift);

    assert(lift->valley_entry == old_valley_entry->follower);
    assert(lift->valley_exit == old_valley_exit->follower);
    assert(lift->middle_upward == old_middle_upward->follower);
    assert(lift->middle_downward == old_middle_downward->follower);
    assert(lift->summit_entry == old_summit_entry->follower);
    assert(lift->summit_exit == old_summit_exit->follower);

    destroy_lift(lift);
    destroy_clock(clock);
}


static void test_move_lift_does_not_advance_outside_operating_hours(void) {
    Clock* clock = init_clock(t(8, 50, 0));
    Lift* lift = create_lift(clock);

    assert(clock != NULL);
    assert(lift != NULL);

    const Gondola* old_valley_entry = lift->valley_entry;
    const Gondola* old_valley_exit = lift->valley_exit;

    move_lift(lift);

    assert(lift->valley_entry == old_valley_entry);
    assert(lift->valley_exit == old_valley_exit);

    destroy_lift(lift);
    destroy_clock(clock);
}


int main(void) {
    test_create_lift_initializes_station_gondolas();
    test_lift_gondola_chain_is_circular();
    test_init_station_gondolas_sets_expected_offsets();
    test_move_lift_advances_station_gondolas_during_operating_hours();
    test_move_lift_does_not_advance_outside_operating_hours();
    return 0;
}
