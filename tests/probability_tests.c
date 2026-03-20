#include <assert.h>
#include <math.h>
#include <skiresort/probability.h>
#define NUM_SAMPLES 10000


static void test_random_int_in_range_stays_within_bounds(void) {
    enable_random();

    for (int i = 0; i < NUM_SAMPLES; i++) {
        const int value = random_int_in_range(3, 7);
        assert(3 <= value);
        assert(value <= 7);
    }
}


static void test_random_double_in_range_stays_within_bounds(void) {
    enable_random();

    for (int i = 0; i < NUM_SAMPLES; i++) {
        const double value = random_double_in_range(1.5, 4.5);
        assert(1.5 <= value);
        assert(value <= 4.5);
    }
}


static void test_random_normal_with_bounds_stays_within_bounds(void) {
    enable_random();

    for (int i = 0; i < NUM_SAMPLES; i++) {
        const double value = random_normal_with_bounds(10.0, 2.0, 5.0, 15.0);
        assert(5.0 <= value);
        assert(value <= 15.0);
    }
}


static void test_random_log_normal_with_max_stays_within_bounds(void) {
    enable_random();

    for (int i = 0; i < NUM_SAMPLES; i++) {
        const double value = random_log_normal_with_max(3.0, 8.0, 1.0, 20.0);
        assert(3.0 <= value);
        assert(value <= 20.0);
    }
}


static void test_box_muller_transform_returns_finite_values(void) {
    const double value = box_muller_transform(0.5, 0.5);
    assert(isfinite(value));
}


int main(void) {
    test_random_int_in_range_stays_within_bounds();
    test_random_double_in_range_stays_within_bounds();
    test_random_normal_with_bounds_stays_within_bounds();
    test_random_log_normal_with_max_stays_within_bounds();
    test_box_muller_transform_returns_finite_values();
    return 0;
}
