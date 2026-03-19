#ifndef LIFT_H
#define LIFT_H
#define TOTAL_GONDOLAS 110
#include "Gondola.h"


/*  Attributes of Lift:
 *   - reference to the main clock
 *   - references to gondolas at valley station
 *   - references to gondolas at middle station
 *   - references to gondolas at summit station
 */
typedef struct Lift {
    Clock* clock;
    Gondola* valley_entry;
    Gondola* valley_exit;
    Gondola* middle_upward;
    Gondola* middle_downward;
    Gondola* summit_entry;
    Gondola* summit_exit;
} Lift;


Lift* create_lift(Clock*);
void add_gondola(Lift*);
void init_station_gondolas(Lift*);
void move_lift(Lift*);
int passengers_on_lift_between(const Gondola*, const Gondola*);
void destroy_lift(Lift*);


#endif /* LIFT_H */
