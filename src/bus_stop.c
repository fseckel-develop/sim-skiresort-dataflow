#include <skiresort/bus_stop.h>
#include <stdlib.h>
#include <stdio.h>


/*  create_bus_stop():
 *  Creates a bus stop associated with the given station and the given clock.
 *      params: reference to station, reference to clock
 *      return: reference to bus stop (or NULL)
 */
BusStop* create_bus_stop(Station* entry, Clock* clock) {
    if (entry && clock) {
        BusStop* bus_stop = calloc(1, sizeof(BusStop));
        if (!bus_stop) {
            fprintf(stderr, "Error in create_bus_stop: failed to allocate memory for bus stop\n");
            return NULL;
        }
        bus_stop->entry = entry;
        bus_stop->clock = clock;
        bus_stop->queue = create_queue();
        if (!bus_stop->queue) {
            fprintf(stderr, "Error in create_bus_stop: failed to create queue for passengers\n");
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
    if (bus_stop && !bus_stop->bus) {
        if (arrival_scheduled(bus_stop->clock->time)) {
            bus_stop->bus = create_bus(bus_stop->clock);
            while (!bus_is_empty(bus_stop->bus)) {
                Person* new_skier = leave_bus(bus_stop->bus);
                plan_on_last_bus(new_skier);
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
    if (time.s == 0) {
        if (time.min == 10 || time.min == 30 || time.min == 50) {
            if (time.h == 8 && time.min == 50) {
                return TRUE;
            }
            if (9 <= time.h && time.h < 20) {
                return TRUE;
            }
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
    if (person) {
        if (person->origin == BUS_STOP && s(t(19,30,00)) < s(person->departure_time)) {
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
    if (bus_stop) {
        const Node* current = bus_stop->entry->people_at_station->front;
        while (current) {
            if (current->person->going_to == BUS_STOP) {
                Person* waiting_passenger = current->person;
                current = current->next;
                remove_person(bus_stop->entry->people_at_station, waiting_passenger);
                enqueue(bus_stop->queue, waiting_passenger);
                continue;
            }
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
    if (bus_stop && bus_stop->bus) {
        while (!queue_is_empty(bus_stop->queue) && !bus_is_full(bus_stop->bus)) {
            Person* new_passenger = dequeue(bus_stop->queue);
            append_list(bus_stop->bus->passengers, new_passenger);
        }
        if (departure_scheduled(bus_stop->clock->time)) {
            destroy_bus(bus_stop->bus);
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
    if (time.s == 0) {
        if (time.min == 13 || time.min == 33 || time.min == 53) {
            if (time.h == 8 && time.min == 53) {
                return TRUE;
            }
            if (9 <= time.h && time.h < 20) {
                return TRUE;
            }
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
    if (bus_stop && s(t(20,13,00)) < s(bus_stop->clock->time)) {
        while (!queue_is_empty(bus_stop->queue)) {
            Person* late_skier = dequeue(bus_stop->queue);
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
    if (bus_stop) {
        bus_stop->entry = NULL;
        bus_stop->clock = NULL;
        destroy_queue(bus_stop->queue);
        destroy_bus(bus_stop->bus);
        free(bus_stop);
    }
}
