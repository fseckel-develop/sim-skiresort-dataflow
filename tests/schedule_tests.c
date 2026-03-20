#include <assert.h>
#include <skiresort/bus_stop.h>
#include <skiresort/bistro.h>
#include <skiresort/lift_queue.h>
#include <skiresort/clock.h>


static void test_bus_arrival_schedule(void) {
    assert(arrival_scheduled(t(8, 50, 0)) == TRUE);
    assert(arrival_scheduled(t(9, 10, 0)) == TRUE);
    assert(arrival_scheduled(t(20, 10, 0)) == TRUE);
    assert(arrival_scheduled(t(20, 30, 0)) == FALSE);
}


static void test_bus_departure_schedule(void) {
    assert(departure_scheduled(t(8, 53, 0)) == TRUE);
    assert(departure_scheduled(t(9, 13, 0)) == TRUE);
    assert(departure_scheduled(t(20, 13, 0)) == TRUE);
    assert(departure_scheduled(t(20, 33, 0)) == FALSE);
}


static void test_bistro_opening_hours(void) {
    assert(bistro_is_open(t(8, 59, 59)) == FALSE);
    assert(bistro_is_open(t(9, 0, 0)) == TRUE);
    assert(bistro_is_open(t(21, 0, 0)) == TRUE);
    assert(bistro_is_open(t(21, 0, 1)) == FALSE);
}


static void test_lift_opening_hours(void) {
    assert(lift_is_open(t(8, 59, 59)) == FALSE);
    assert(lift_is_open(t(9, 0, 0)) == TRUE);
}


int main(void) {
    test_bus_arrival_schedule();
    test_bus_departure_schedule();
    test_bistro_opening_hours();
    test_lift_opening_hours();
    return 0;
}
