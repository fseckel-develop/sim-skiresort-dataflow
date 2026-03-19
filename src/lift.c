#define INIT_POS_MIDDLE_DOWNWARD 25
#define INIT_POS_SUMMIT_ENTRY    55
#define INIT_POS_SUMMIT_EXIT     56
#define INIT_POS_MIDDLE_UPWARD   86
#include <skiresort/lift.h>
#include <stdlib.h>
#include <stdio.h>


/*  create_lift():
 *  Creates a lift and associates it with the given clock. After initially
 *  nullifying the gondola references for memory safety, gondolas are added
 *  and the station gondolas are initialised.
 *      params: reference to clock
 *      return: reference to lift (or NULL)
 */
Lift* create_lift(Clock* clock) {
    if (clock) {
        int i;
        Lift* lift = calloc(1, sizeof(Lift));
        if (!lift) {
            fprintf(stderr, "Error in create_lift: failed to allocate memory for lift\n");
            return NULL;
        }
        lift->clock = clock;
        for (i = 0; i < TOTAL_GONDOLAS; i++) {
            add_gondola(lift);
        }
        init_station_gondolas(lift);
        return lift;
    }
    return NULL;
}


/*  add_gondola():
 *  Creates a new gondola and integrates it into the lift, between its
 *  current valley entry and valley exit gondolas, ultimately building
 *  up a circular singly-connected list of gondolas.
 *      params: reference to lift
 *      return: none
 */
void add_gondola(Lift* lift) {
    /* Building up of the circular linked list starts at the valley station. The very first
     * gondola being added will also be set as its own follower, resulting in a circle closure.
     * The first gondola will stay marked as the valley exit. Every new gondola is inserted as
     * follower of the current valley entry, then set to the valley entry itself and connected
     * to the valley exit as its follower, maintaining the circle closure. */

    if (lift) {
        Gondola* new_gondola = create_gondola();
        /* Is valley entry gondola already set? = Is it not the first gondola being added? */
        if (lift->valley_entry) {
            /* Setting new gondola as the follower of valley entry gondola */
            lift->valley_entry->follower = new_gondola;
        } else {
            /* Setting new gondola as the valley exit gondola, marking circle closure */
            lift->valley_exit = new_gondola;
        }
        /* Updating valley entry to the new gondola */
        lift->valley_entry = new_gondola;
        /* Setting valley exit as the follower of the new gondola, maintaining circle closure */
        lift->valley_entry->follower = lift->valley_exit;
    }
}


/*  init_station_gondolas():
 *  Setting the references of the according exit/entry gondolas for the given lift. A gondola
 *  needs 4 minutes from middle downward to valley exit, which is equal to 4 * 60s = 240s.
 *  Every 10 seconds a new gondola arrives at a station, therefore 240s / 10s = 24 gondolas
 *  are passing before the gondola in question is arriving at the station. So when gondola #1
 *  is at valley exit, then gondola #25 is located at middle station, downwards direction.
 *  Position of other initial station gondolas determined the same way.
 *      params: reference to lift
 *      return: none
 */
void init_station_gondolas(Lift* lift) {
    if (lift) {
        Gondola* current = lift->valley_exit;
        int gondola_counter = 1;
        while (gondola_counter <= INIT_POS_MIDDLE_UPWARD) {
            if (gondola_counter == INIT_POS_MIDDLE_DOWNWARD) {
                lift->middle_downward = current;
            }
            else if (gondola_counter == INIT_POS_SUMMIT_ENTRY) {
                lift->summit_entry = current;
            }
            else if (gondola_counter == INIT_POS_SUMMIT_EXIT) {
                lift->summit_exit = current;
            }
            else if (gondola_counter == INIT_POS_MIDDLE_UPWARD) {
                lift->middle_upward = current;
            }
            current = current->follower;
            gondola_counter++;
        }
    }
}


/*  move_lift():
 *  Sets each station pointer from the current gondola to its following gondola
 *  during the time from 9 am to 8 pm, simulating movement of the given lift.
 *      params: reference to lift
 *      return: none
 */
void move_lift(Lift* lift) {
    if (lift) {
        /* Is the lift currently moving? */
        if (s(t(9,0,0)) <= s(lift->clock->time) && s(lift->clock->time) <= s(t(20,0,0))) {
            /* Updating every station gondola to the next following gondola */
            lift->valley_entry = lift->valley_entry->follower;
            lift->valley_exit = lift->valley_exit->follower;
            lift->middle_upward = lift->middle_upward->follower;
            lift->middle_downward = lift->middle_downward->follower;
            lift->summit_entry = lift->summit_entry->follower;
            lift->summit_exit = lift->summit_exit->follower;
        }
    }
}


/*  passengers_on_lift_between():
 *  Counts the passengers on the lift between the two given gondolas.
 *      params: reference to ending gondola, reference to starting gondola
 *      return: number of passengers
 */
int passengers_on_lift_between(const Gondola* ending_gondola, const Gondola* starting_gondola) {
    int counter = 0;
    if (ending_gondola && starting_gondola) {
        const Gondola* current = starting_gondola;
        do {
            counter += current->passenger_count;
            current = current->follower;
        } while (current != ending_gondola);
    }
    return counter;
}


/*  destroy_lift():
 *  Nullifies the association to the clock, destroys all gondolas after
 *  cutting the circular list and frees the memory of the given hotel.
 *      params: reference to lift
 *      return: none
 */
void destroy_lift(Lift* lift) {
    if (lift) {
        Gondola* current = lift->valley_exit;
        /* Cutting the circular list after the last traversed gondola */
        lift->valley_entry->follower = NULL;
        while (current) {
            Gondola* temp = current;
            current = current->follower;
            destroy_gondola(temp);
        }
        lift->clock = NULL;
        free(lift);
    }
}
