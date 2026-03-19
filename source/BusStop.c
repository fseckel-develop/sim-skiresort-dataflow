#include "BusStop.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_bus_stop():
 *  Creates a bus stop associated with the given station and the given clock.
 *      params: reference to station, reference to clock
 *      return: reference to bus stop (or NULL)
 */
BusStop* create_bus_stop(Station* entry, Clock* clock) {
    /* Are the given station reference and clock reference valid? */
    if (entry && clock) {
        /* Allocating memory for a BusStop struct */
        BusStop* bus_stop = calloc(1, sizeof(BusStop));
        /* Has the memory allocation been unsuccessful? */
        if (!bus_stop) {
            fprintf(stderr, "Error in create_bus_stop: failed to allocate memory for bus stop\n");
            return NULL;
        }
        /* Associating bus stop with the given station */
        bus_stop->entry = entry;
        /* Associating bus stop with the given clock */
        bus_stop->clock = clock;
        /* Creating a queue for waiting bus passengers */
        bus_stop->queue = create_queue();
        /* Has the queue creation been unsuccessful? */
        if (!bus_stop->queue) {
            fprintf(stderr, "Error in create_bus_stop: failed to create queue for passengers\n");
            /* Destroying bus stop cause of incorrect resources */
            destroy_bus_stop(bus_stop);
            return NULL;
        }
        return bus_stop;
    }
    return NULL;
}


/*  bus_is_arriving():
 *  Creates a filled bus at scheduled arrival times and moves all
 *  passengers to the station associated with the given bus stop.
 *      params: reference to bus stop
 *      return: none
 */
void bus_is_arriving(BusStop* bus_stop) {
    /* Is the given bus stop reference valid and is no bus present? */
    if (bus_stop && !bus_stop->bus) {
        /* Is a bus arrival scheduled at the current time? */
        if (arrival_scheduled(bus_stop->clock->time)) {
            /* Creating a new bus with passengers */
            bus_stop->bus = create_bus(bus_stop->clock);
            /* Repeating until the bus is empty: */
            while (!bus_is_empty(bus_stop->bus)) {
                /* Extracting the next new skier from the bus */
                Person* new_skier = leave_bus(bus_stop->bus);
                /* Rescheduling departure time of the new skier eventually */
                plan_on_last_bus(new_skier);
                /* Moving new skier to the entry station */
                go_to_station(bus_stop->entry, new_skier);
            }
        }
    }
}


/*  arrival_scheduled():
 *  Checks if a bus arrival is scheduled at a given time.
 *  Buses are arriving every 20 minutes from 8:50 am to 8:10 pm.
 *      params: time
 *      return: TRUE or FALSE
 */
Boolean arrival_scheduled(const Time time) {
    /* Has the current time reached a full minute? */
    if (time.s == 0) {
        /* Does the current time measure 10, 30 or 50 minutes? */
        if (time.min == 10 || time.min == 30 || time.min == 50) {
            /* Is it 8:50 am? */
            if (time.h == 8 && time.min == 50) {
                return TRUE;
            }
            /* Is it between 9:00 am and 8:00 pm? */
            if (9 <= time.h && time.h < 20) {
                return TRUE;
            }
            /* Is it 8:10 pm? */
            if (time.h == 20 && time.min == 10) {
                return TRUE;
            }
        }
    }
    return FALSE;
}


/*  plan_on_last_bus():
 *  Clamps the departure time of a given person if their initial departure time is
 *  later than 7:30 pm, ensuring buffer time to get the last bus coming at 8:10 pm.
 *      params: reference to person
 *      return: none
 */
void plan_on_last_bus(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Has the person arrived by bus and does it initially plan to leave after 7:30 pm? */
        if (person->origin == BUS_STOP && s(t(19,30,00)) < s(person->departure_time)) {
            /* Setting departure time of the person to 7:30 pm */
            person->departure_time = t(19,30,00);
        }
    }
}


/*  return_to_bus_stop():
 *  Moves people from the associated station to the given bus stop if it is
 *  their next destination. People are put into the queue of the bus stop.
 *      params: reference to bus stop
 *      return: none
 */
void return_to_bus_stop(const BusStop* bus_stop) {
    /* Is the given bus stop reference valid? */
    if (bus_stop) {
        /* Preparing traversal of entry stations person list */
        const Node* current = bus_stop->entry->people_at_station->front;
        while (current) {
            /* Does the current person want to return to the bus stop? */
            if (current->person->going_to == BUS_STOP) {
                /* Saving reference to current person */
                Person* waiting_passenger = current->person;
                /* Progressing list traversal for safe element removal */
                current = current->next;
                /* Removing returning passenger from entry stations person list */
                remove_person(bus_stop->entry->people_at_station, waiting_passenger);
                /* Adding returning passenger to the end of the bus stops queue */
                enqueue(bus_stop->queue, waiting_passenger);
                continue;
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  bus_is_departing():
 *  Moves people from the queue of the given bus stop to an existing bus as long as
 *  it is not full. When a scheduled departure time is reached, the bus is destroyed.
 *      params: reference to bus stop
 *      return: none
 */
void bus_is_departing(BusStop* bus_stop) {
    /* Is the given bus stop reference valid and is a bus present? */
    if (bus_stop && bus_stop->bus) {
        /* Repeating until the bus stop queue is empty or the bus is full: */
        while (!queue_is_empty(bus_stop->queue) && !bus_is_full(bus_stop->bus)) {
            /* Extracting new passenger from the front of the bus stop queue */
            Person* new_passenger = dequeue(bus_stop->queue);
            /* Adding new passenger to the bus passenger list */
            append_list(bus_stop->bus->passengers, new_passenger);
        }
        /* Is a bus departure scheduled at the current time? */
        if (departure_scheduled(bus_stop->clock->time)) {
            /* Removing bus from the bus stop by destroying it */
            destroy_bus(bus_stop->bus);
            /* Nullifying the parking spot (no bus at the bus stop) */
            bus_stop->bus = NULL;
        }
    }
}


/*  departure_scheduled():
 *  Checks if a bus departure is scheduled at a given time.
 *  Buses are departing every 20 minutes from 8:53 am to 8:13 pm.
 *      params: time
 *      return: TRUE or FALSE
 */
Boolean departure_scheduled(const Time time) {
    /* Has the current time reached a full minute? */
    if (time.s == 0) {
        /* Does the current time measure 13, 33 or 53 minutes? */
        if (time.min == 13 || time.min == 33 || time.min == 53) {
            /* Is it 8:53 am? */
            if (time.h == 8 && time.min == 53) {
                return TRUE;
            }
            /* Is it between 9:00 am and 8:00 pm? */
            if (9 <= time.h && time.h < 20) {
                return TRUE;
            }
            /* Is it 8:13 pm? */
            if (time.h == 20 && time.min == 13) {
                return TRUE;
            }
        }
    }
    return FALSE;
}


/*  get_taxi_for_late_bus_passengers():
 *  Destroys all people left in the queue of the given
 *  bus stop in case they missed the last bus at 8:13 pm.
 *      params: reference to bus stop
 *      return: none
 */
void get_taxi_for_late_bus_passengers(const BusStop* bus_stop) {
    /* Is the given bus stop reference valid and is it past the last bus departure at 8:13 pm? */
    if (bus_stop && s(t(20,13,00)) < s(bus_stop->clock->time)) {
        /* Repeating until bus stop queue is empty: */
        while (!queue_is_empty(bus_stop->queue)) {
            /* Extracting leaving person from the front of the bus stop queue */
            Person* late_skier = dequeue(bus_stop->queue);
            /* Freeing memory of leaving person */
            free(late_skier);
        }
    }
}


/*  destroy_bus_stop():
 *  Nullifies the association to the station and clock, destroys the queue of
 *  passengers and any existing bus and frees the memory of the given bus stop.
 *      params: reference to bus stop
 *      return: none
 */
void destroy_bus_stop(BusStop* bus_stop) {
    /* Is the given bus stop reference valid? */
    if (bus_stop) {
        /* Breaking association with entry station */
        bus_stop->entry = NULL;
        /* Breaking association with clock */
        bus_stop->clock = NULL;
        /* Destroying queue of waiting passengers */
        destroy_queue(bus_stop->queue);
        /* Destroying bus in case one is present */
        destroy_bus(bus_stop->bus);
        /* Freeing memory of the given bus stop */
        free(bus_stop);
    }
}
