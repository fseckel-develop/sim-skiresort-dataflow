#ifndef BISTRO_H
#define BISTRO_H
#include <skiresort/station.h>


/*  Attributes of Bistro:
 *   - reference to station at the bistro
 *   - reference to list of customers
 */
typedef struct Bistro {
    Station* entry;
    List* customers;
} Bistro;


Bistro* create_bistro(Station*);
void welcome_customers(const Bistro*);
Boolean bistro_is_open(Time);
void set_time_to_eat(Person*);
void customers_are_eating(const Bistro*);
void destroy_bistro(Bistro*);


#endif /* BISTRO_H */
