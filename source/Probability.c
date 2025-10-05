#include "Probability.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


/*  enable_random():
 *  Seeds the needed pseudo-random number generator
 *  with a distinctive runtime value using time(NULL).
 *      params: None
 *      return: None
 */
void enable_random(void) {
    srand(time(NULL));
}


/*  random_int_in_range():
 *  Determines a random integer value within a given range.
 *      params: minimum value, maximum value
 *      return: integer value
 */
int random_int_in_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}


/*  random_double_in_range():
 *  Determines a random floating point value within a given range.
 *      params: minimum double value, maximum double value
 *      return: double value
 */
double random_double_in_range(const double min, const double max) {
    return (double) rand() / (double) RAND_MAX * (max - min) + min;
}


/*  box_muller_transform():
 *  Determines a value following a normal distribution with mean = 0 and sigma = 1
 *  by using the box muller transformation on two given values, ideally given from
 *  the uniform distribution in the range [0, 1], as used in following functions.
 *      params: two double values
 *      return: double value
 */
double box_muller_transform(const double u1, const double u2) {
    return sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
}


/*  random_normal():
 *  Determines a value following a normal distribution with the given mean and given sigma.
 *      params: mean (average), sigma (standard deviation)
 *      return: double value
 */
double random_normal(const double mean, const double sigma) {
    /* Pulling two uniformly distributed numbers within the range [0, 1] */
    const double u1 = random_double_in_range(0.0, 1.0);
    const double u2 = random_double_in_range(0.0, 1.0);
    /* Calculating normally distributed number using the Box-Muller transform */
    return mean + box_muller_transform(u1, u2) * sigma;
}


/*  random_normal_with_bounds():
 *  Determines a value following a normal distribution with the given mean
 *  and given sigma, respecting the given minimal and maximal value.
 *      params: mean, sigma, minimum, maximum
 *      return: double value
 */
double random_normal_with_bounds(const double mean, const double sigma, const double min, const double max) {
    double result;
    /* Repeating until the result is within the given range [min, max]: */
    do {
        /* Pulling two uniformly distributed numbers within the range [0, 1] */
        const double u1 = random_double_in_range(0.0, 1.0);
        const double u2 = random_double_in_range(0.0, 1.0);
        /* Calculating normally distributed number using the Box-Muller transform */
        result = mean + box_muller_transform(u1, u2) * sigma;
    } while (result < min || max < result);
    return result;
}


/*  random_log_normal():
 *  Determines a value following a log-normal distribution with
 *  the given mean, given sigma and given minimal value.
 *      params: minimum, mean, sigma
 *      return: double value
 */
double random_log_normal(const double min, const double mean, const double sigma) {
    /* Computing the mean value shifted by the minimum value */
    const double shifted_mean = mean - min;
    /* Converting the shifted mean to the log-normal distributions mean */
    const double log_mean = log(shifted_mean) - sigma * sigma / 2.0;
    /* Generating log-normal distributed random value and shift it back */
    return min + exp(random_normal(log_mean, sigma));
}


/*  random_log_normal_with_max():
 *  Determines a value following a log-normal distribution with the given mean,
 *  given sigma and given minimal value, respecting the given maximal value.
 *      params: minimum, mean, sigma, maximum
 *      return: double value
 */
double random_log_normal_with_max(const double min, const double mean, const double sigma, const double max) {
    /* Computing the mean value shifted by the minimum value */
    const double shifted_mean = mean - min;
    /* Convert the shifted mean to the mean of the log-normal distribution */
    const double log_mean = log(shifted_mean) - sigma * sigma / 2.0;
    double result;
    /* Repeating until the result is within the range [min, max]: */
    do {
        /* Generating log-normal distributed value and shift it back */
        result = min + exp(random_normal(log_mean, sigma));
    }
    while (result > max);
    return result;
}
