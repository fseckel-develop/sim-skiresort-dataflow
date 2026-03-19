#include "Bus.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_bus():
 *  Creates a bus and fills it with passengers providing them with the given clock.
 *      params: reference to clock
 *      return: reference to bus (or NULL)
 */
Bus* create_bus(Clock* clock) {
    if (clock) {
        Bus* bus = calloc(1, sizeof(Bus));
        if (!bus) {
            fprintf(stderr, "Error in create_bus: failed to allocate memory for bus\n");
            return NULL;
        }
        bus->passenger_count = init_passenger_count(clock->time);
        bus->passengers = create_list();
        if (!bus->passengers) {
            fprintf(stderr, "Error in create_bus: failed to create list of passengers\n");
            destroy_bus(bus);
            return NULL;
        }
        fill_bus(bus, clock);
        return bus;
    }
    return NULL;
}


/*  init_passenger_count():
 *  Determines a random passenger count based on the given arrival time.
 *      params: time
 *      return: number of passengers
 */
int init_passenger_count(const Time time) {
    if (arrival_density(time) == 0.0) {
        return 0;
    }
    /* Passenger count normally distributed with mean between 0.0 and 47.0 (sigma = 2.0) */
    return (int) random_normal_with_bounds(47.0 * arrival_density(time), 2.0, 0.0, BUS_CAPACITY);
}


/*  fill_bus():
 *  Creates persons provided with the given clock and
 *  appends them to the passenger list of the given bus.
 *      params: reference to bus, reference to clock
 *      return: none
 */
void fill_bus(const Bus* bus, Clock* clock) {
    if (bus) {
        int i;
        for (i = 0; i < bus->passenger_count; i++) {
            Person* new_passenger = create_person(BUS_STOP, clock);
            append_list(bus->passengers, new_passenger);
        }
    }
}


/*  leave_bus():
 *  Gets the next passenger of the given bus and returns it.
 *      params: reference to bus
 *      return: reference to person (or NULL)
 */
Person* leave_bus(const Bus* bus) {
    if (bus && !list_is_empty(bus->passengers)) {
        Person* new_skier = bus->passengers->front->person;
        remove_person(bus->passengers, new_skier);
        return new_skier;
    }
    return NULL;
}


/*  bus_is_full():
 *  Checks of the maximal capacity of 50 is reached for the given bus.
 *      params: reference to bus
 *      return: TRUE or FALSE
 */
Boolean bus_is_full(const Bus* bus) {
    if (bus) {
        return bus->passengers->size == BUS_CAPACITY;
    }
    return FALSE;
}


/*  bus_is_empty():
 *  Checks if the given bus is empty.
 *      params: reference to bus
 *      return: TRUE or FALSE
 */
Boolean bus_is_empty(const Bus* bus) {
    if (bus) {
        return list_is_empty(bus->passengers);
    }
    return FALSE;
}


/*  destroy_bus():
 *  Destroys the passenger list and frees the memory of the given bus.
 *      params: reference to bus
 *      return: none
 */
void destroy_bus(Bus* bus) {
    if (bus) {
        destroy_list(bus->passengers);
        free(bus);
    }
}
