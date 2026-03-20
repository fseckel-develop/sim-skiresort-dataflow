#include <assert.h>
#include <stddef.h>
#include <skiresort/clock.h>


static void test_time_normalization(void) {
    const Time time = t(1, 61, 70);
    assert(time.h == 2);
    assert(time.min == 2);
    assert(time.s == 10);
}


static void test_add_time(void) {
    const Time a = t(1, 30, 30);
    const Time b = t(0, 40, 40);
    const Time result = add(a, b);

    assert(result.h == 2);
    assert(result.min == 11);
    assert(result.s == 10);
}


static void test_min_and_seconds(void) {
    const Time time = t(2, 3, 4);
    assert(min(time) == 123);
    assert(s(time) == 7384);
}


static void test_tick_tack_seconds(void) {
    Clock* clock = init_clock(t(8, 40, 0));
    assert(clock != NULL);

    tick_tack(clock);
    assert(clock->time.h == 8);
    assert(clock->time.min == 40);
    assert(clock->time.s == 10);

    destroy_clock(clock);
}


static void test_tick_tack_rollover(void) {
    Clock* clock = init_clock(t(8, 59, 50));
    assert(clock != NULL);

    tick_tack(clock);
    assert(clock->time.h == 9);
    assert(clock->time.min == 0);
    assert(clock->time.s == 0);

    destroy_clock(clock);
}


int main(void) {
    test_time_normalization();
    test_add_time();
    test_min_and_seconds();
    test_tick_tack_seconds();
    test_tick_tack_rollover();
    return 0;
}
