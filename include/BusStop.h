#ifndef BUSSTOP_H
#define BUSSTOP_H
#include "Station.h"
#include "Queue.h"
#include "Bus.h"


/*  Attributes of BusStop:
 *   - reference to resort station at the bus stop
 *   - reference to the main clock
 *   - reference to queue of waiting people
 *   - reference to bus
 */
typedef struct BusStop {
    Station* entry;
    Clock* clock;
    Queue* queue;
    Bus* bus;
} BusStop;


BusStop* create_bus_stop(Station*, Clock*);
void bus_is_arriving(BusStop*);
Boolean arrival_scheduled(Time);
void plan_on_last_bus(Person*);
void return_to_bus_stop(const BusStop*);
void bus_is_departing(BusStop*);
Boolean departure_scheduled(Time);
void get_taxi_for_late_bus_passengers(const BusStop*);
void destroy_bus_stop(BusStop*);


#endif /* BUSSTOP_H */
