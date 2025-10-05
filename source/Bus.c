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
    /* Is the given clock reference valid? */
    if (clock) {
        /* Allocating memory for a Bus struct */
        Bus* bus = malloc(sizeof(Bus));
        /* Has the memory allocation been unsuccessful? */
        if (!bus) {
            fprintf(stderr, "Error in create_bus: failed to allocate memory for bus\n");
            return NULL;
        }
        /* Setting initial passenger count depending on current time */
        bus->passenger_count = init_passenger_count(clock->time);
        /* Creating a list for car passengers */
        bus->passengers = create_list();
        /* Has the list creation been unsuccessful? */
        if (!bus->passengers) {
            fprintf(stderr, "Error in create_bus: failed to create list of passengers\n");
            /* Destroying bus cause of incorrect resources */
            destroy_bus(bus);
            return NULL;
        }
        /* Filling bus with passengers */
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
    /* Are their no arrivals at this time? */
    if (arrival_density(time) == 0.0) {
        return 0;
    }
    /* Passenger count normally distributed with mean between 0.0 and 47.0 (sigma = 2.0) */
    return (int) random_normal_with_bounds(47.0 * arrival_density(time), 2.0, 0.0, 50.0);
}


/*  fill_bus():
 *  Creates persons provided with the given clock and
 *  appends them to the passenger list of the given bus.
 *      params: reference to bus, reference to clock
 *      return: none
 */
void fill_bus(const Bus* bus, Clock* clock) {
    /* Are the given bus reference and clock reference valid? */
    if (bus) {
        int i;
        /* Repeating for every passenger of the bus: */
        for (i = 0; i < bus->passenger_count; i++) {
            /* Creating new person as passenger of the bus */
            Person* new_passenger = create_person(BUS_STOP, clock);
            /* Adding new passenger to bus passenger list */
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
    /* Is the given bus reference valid and is the bus not empty? */
    if (bus && !list_is_empty(bus->passengers)) {
        /* Saving reference to first passenger of list for return */
        Person* new_skier = bus->passengers->front->person;
        /* Removing passenger from bus passenger list */
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
    /* Is the given bus reference valid? */
    if (bus) {
        /* Has the bus reached its full capacity? */
        return bus->passengers->size == 50;
    }
    return FALSE;
}


/*  bus_is_empty():
 *  Checks if the given bus is empty.
 *      params: reference to bus
 *      return: TRUE or FALSE
 */
Boolean bus_is_empty(const Bus* bus) {
    /* Is the given bus reference valid? */
    if (bus) {
        /* Is the bus passenger list empty? */
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
    /* Is the given bus reference valid? */
    if (bus) {
        /* Destroying list of passengers */
        destroy_list(bus->passengers);
        /* Freeing memory of the given bus */
        free(bus);
    }
}
