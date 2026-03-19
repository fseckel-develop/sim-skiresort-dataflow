#ifndef PROBABILITY_H
#define PROBABILITY_H


void enable_random(void);
int random_int_in_range(int, int);
double random_double_in_range(double, double);
double box_muller_transform(double, double);
double random_normal(double, double);
double random_normal_with_bounds(double, double, double, double);
double random_log_normal(double, double, double);
double random_log_normal_with_max(double, double, double, double);


#endif /* PROBABILITY_H */
