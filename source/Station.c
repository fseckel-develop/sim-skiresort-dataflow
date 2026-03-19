#include "Station.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_station():
 *  Creates a station with the given id which associates it with the suitable function for
 *  directing people at the station. The entry and exit gondola are set via the given lift.
 *      params: resort part id, reference to lift
 *      return: reference to station (or NULL)
 */
Station* create_station(const Position id, const Lift* lift) {
    /* Is the given lift reference valid? */
    if (lift) {
        /* Allocating memory for a Station struct */
        Station* station = calloc(1, sizeof(Station));
        /* Has the memory allocation been unsuccessful? */
        if (!station) {
            fprintf(stderr, "Error in create_station: failed to allocate memory for station\n");
            return NULL;
        }
        /* Setting the station id to the given id */
        station->id = id;
        /* Creating a list for people at the station */
        station->people_at_station = create_list();
        /* Has the list creation been unsuccessful? */
        if (!station->people_at_station) {
            fprintf(stderr, "Error in create_station: failed to create list for people\n");
            /* Destroying station cause of incorrect resources */
            destroy_station(station);
            return NULL;
        }
        /* Which of the possible stations is it? */
        switch (station->id) {
            case VALLEY_STATION: {
                /* Associating directive function of valley station */
                station->direct_person = direct_people_at_valley_station;
                break;
            }
            case MIDDLE_STATION: {
                /* Associating directive function of middle station */
                station->direct_person = direct_people_at_middle_station;
                break;
            }
            case SUMMIT_STATION: {
                /* Associating directive function of summit station */
                station->direct_person = direct_people_at_summit_station;
                break;
            }
            default: {}
        }
        /* Setting lift gondolas at the station */
        set_gondolas(station, lift);
        return station;
    }
    return NULL;
}


/*  update_gondolas():
 *  Depending on the id of the given station it is associated with a specific
 *  pair of entry gondola and exit gondola provided by the given lift.
 *      params: reference to station, reference to lift
 *      return: none
 */
void set_gondolas(Station* station, const Lift* lift) {
    /* Are the given station reference and lift reference valid? */
    if (station && lift) {
        /* Which of the possible stations is it? */
        switch (station->id) {
            case VALLEY_STATION: {
                /* Associating valley gondolas with the station */
                station->upward_gondola = lift->valley_entry;
                station->downward_gondola = lift->valley_exit;
                break;
            }
            case MIDDLE_STATION: {
                /* Associating middle gondolas with the station */
                station->upward_gondola = lift->middle_upward;
                station->downward_gondola = lift->middle_downward;
                break;
            }
            case SUMMIT_STATION: {
                /* Associating summit gondolas with the station */
                station->upward_gondola = lift->summit_exit;
                station->downward_gondola = lift->summit_entry;
                break;
            }
            default: {}
        }
    }
}


/*  exit_lift():
 *  Moves all lift passengers from the associated upward and downward
 *  gondola to the given station if it is their next destination.
 *      params: reference to station
 *      return: none
 */
void exit_lift(const Station* station) {
    /* Is the given station reference valid? */
    if (station) {
        int i;
        /* Repeating for each of the station gondolas seats: */
        for (i = 0; i < 4; i++) {
            /* Saving reference to the person in upward gondola (or NULL) */
            Person* passenger = station->upward_gondola->seat[i];
            /* Is the reference valid and does the person want to exit at this station? */
            if (passenger && passenger->going_to == station->id) {
                /* Person is exiting the gondola */
                exit_gondola(station->upward_gondola, passenger);
                /* Moving exited passenger to the station */
                go_to_station(station, passenger);
            }
            /* Saving reference to the person in downward gondola (or NULL) */
            passenger = station->downward_gondola->seat[i];
            /* Is the reference valid and does the person want to exit at this station? */
            if (passenger && passenger->going_to == station->id) {
                /* Person is exiting the gondola */
                exit_gondola(station->downward_gondola, passenger);
                /* Moving exited passenger to the station */
                go_to_station(station, passenger);
            }
        }
    }
}


/*  go_to_station():
 *  Adds the given person to the list of people at the given station and
 *  directs it to their next destination reachable from that station.
 *      params: reference to station, reference to person
 *      return: none
 */
void go_to_station(const Station* station, Person* person) {
    /* Are the given station reference and person reference valid? */
    if (station && person) {
        /* Adding the person to the stations list of people */
        append_list(station->people_at_station, person);
        /* Directing person to their next destination dependent on the station */
        station->direct_person(person);
    }
}


/*  direct_people_at_valley_station():
 *  Sets the next destination of the given person to their origin if the time is past their
 *  departure time or if they are coming back from the valley lift queue cause of the lift
 *  already being closed. Otherwise, they are directed to the valley lift queue.
 *      params: reference to person
 *      return: none
 */
void direct_people_at_valley_station(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Does person not yet want to depart and were they not recently redirected at valley lift queue? */
        if (s(person->clock->time) < s(person->departure_time) && person->going_to != VALLEY_LIFT_QUEUE) {
            /* Person wants to enter the lift */
            person->going_to = VALLEY_LIFT_QUEUE;
        }
        else {
            /* Person wants to go to the resort part where they arrived */
            person->going_to = person->origin;
        }
    }
}


/*  direct_people_at_middle_station():
 *  Randomly sets the next destination of the given person to the bistro, one of the middle
 *  lift queues or one of the slopes depending on the skill level of the person. If the person
 *  is coming back from one of these destinations (due to a finished bistro visit or cause of
 *  the lift already being closed) these destinations are ignored in the random choice.
 *        ,---------------,---------------,---------------,---------------,---------------,
 *        | Destination:  | Frightened:   | Beginner:     | Intermediate: | Advanced:     |
 *        | Bistro        | 40%           | 20%           | 20%           | 20%           |
 *        | Queue Up      | 10%           | 20%           | 30%           | 30%           |
 *        | Queue Down    | 30%           | 10%           |  0%           |  0%           |
 *        | Slope B1      | 20%           | 40%           | 20%           | 10%           |
 *        | Slope R1      |  0%           | 10%           | 30%           | 40%           |
 *        '---------------'---------------'---------------'---------------'---------------'
 *      params: reference to person
 *      return: none
 */
void direct_people_at_middle_station(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Pulling random integer between 0 and 100, uniformly distributed */
        const int random = random_int_in_range(1, 100);
        /* Which skill level does the person have? */
        switch (person->skill_level) {
            case FRIGHTENED: {
                /* Is random number between 1 and 40 (40% chance)
                 * and was person not recently redirected at bistro? */
                if (random <= 40 && person->going_to != BISTRO) {
                    /* Directing person to bistro */
                    person->going_to = BISTRO;
                }
                /* Is random number between 41 and 50 (10% chance) and was person not
                 * recently redirected at the upward lift queue at middle station? */
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    /* Directing person to upward lift queue at middle station */
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                /* Is random number between 51 and 80 (30% chance) and was person not
                 * recently redirected at the downward lift queue at middle station? */
                else if (random <= 80 && person->going_to != MIDDLE_LIFT_QUEUE_DOWN) {
                    /* Directing person to downward lift queue at middle station */
                    person->going_to = MIDDLE_LIFT_QUEUE_DOWN;
                }
                /* Random number is between 81 and 100 (20% chance) */
                else {
                    /* Directing person to slope B1 */
                    person->going_to = SLOPE_B1;
                }
                break;
            }
            case BEGINNER: {
                /* Is random number between 1 and 20 (20% chance)
                 * and was person not recently redirected at bistro? */
                if (random <= 20 && person->going_to != BISTRO) {
                    /* Directing person to bistro */
                    person->going_to = BISTRO;
                }
                /* Is random number between 21 and 40 (30% chance) and was person not
                 * recently redirected at the upward lift queue at middle station? */
                else if (random <= 40 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    /* Directing person to upward lift queue at middle station */
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                /* Is random number between 41 and 50 (10% chance) and was person not
                 * recently redirected at the downward lift queue at middle station? */
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_DOWN) {
                    /* Directing person to downward lift queue at middle station */
                    person->going_to = MIDDLE_LIFT_QUEUE_DOWN;
                }
                /* Is random number between 51 and 90? (40% chance) */
                else if (random <= 90) {
                    /* Directing person to slope B1 */
                    person->going_to = SLOPE_B1;
                }
                /* Random number is between 91 and 100 (10% chance) */
                else {
                    /* Directing person to slope R1 */
                    person->going_to = SLOPE_R1;
                }
                break;
            }
            case INTERMEDIATE: {
                /* Is random number between 1 and 20 (20% chance)
                 * and was person not recently redirected at bistro? */
                if (random <= 20 && person->going_to != BISTRO) {
                    /* Directing person to bistro */
                    person->going_to = BISTRO;
                }
                /* Is random number between 21 and 50 (30% chance) and was person not
                 * recently redirected at the upward lift queue at middle station? */
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    /* Directing person to upward lift queue at middle station */
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                /* Is random number between 51 and 70? (20% chance) */
                else if (random <= 70) {
                    /* Directing person to slope B1 */
                    person->going_to = SLOPE_B1;
                }
                /* Random number is between 71 and 100 (30% chance) */
                else {
                    /* Directing person to slope R1 */
                    person->going_to = SLOPE_R1;
                }
                break;
            }
            case ADVANCED: {
                /* Is random number between 1 and 20 (20% chance)
                 * and was person not recently redirected at bistro? */
                if (random <= 20 && person->going_to != BISTRO) {
                    /* Directing person to bistro */
                    person->going_to = BISTRO;
                }
                /* Is random number between 21 and 50 (30% chance) and was person not
                 * recently redirected at the upward lift queue at middle station? */
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    /* Directing person to upward lift queue at middle station */
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                /* Is random number between 51 and 60? (10% chance) */
                else if (random <= 60) {
                    /* Directing person to slope B1 */
                    person->going_to = SLOPE_B1;
                }
                /* Random number is between 61 and 100 (40% chance) */
                else {
                    /* Directing person to slope R1 */
                    person->going_to = SLOPE_R1;
                }
                break;
            }
            default: {}
        }
    }
}


/*  direct_people_at_summit_station():
 *  Randomly sets the next destination of the given person to the summit lift queue or one of
 *  the slopes depending on the skill level of the person. If the person is coming back from
 *  lift queue (cause the lift already being closed) it is ignored in the random choice.
 *        ,---------------,---------------,---------------,---------------,---------------,
 *        | Destination:  | Frightened:   | Beginner:     | Intermediate: | Advanced:     |
 *        | Queue         | 80%           | 20%           |  0%           |  0%           |
 *        | Slope B2      | 20%           | 60%           | 20%           | 10%           |
 *        | Slope R2      |  0%           | 20%           | 60%           | 30%           |
 *        | Slope S1      |  0%           |  0%           | 20%           | 60%           |
 *        '---------------'---------------'---------------'---------------'---------------'
 *      params: reference to person
 *      return: none
 */
void direct_people_at_summit_station(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Pulling random integer between 0 and 100, uniformly distributed */
        const int random = random_int_in_range(1, 100);
        /* Which skill level does the person have? */
        switch (person->skill_level) {
            case FRIGHTENED: {
                /* Is random number between 1 and 80 (80% chance) and was person
                 * not recently redirected at the lift queue at summit station? */
                if (random <= 80 && person->going_to != SUMMIT_LIFT_QUEUE) {
                    person->going_to = SUMMIT_LIFT_QUEUE;
                }
                /* Random number is between 81 and 100 (20% chance) */
                else {
                    /* Directing person to slope B2 */
                    person->going_to = SLOPE_B2;
                }
                break;
            }
            case BEGINNER: {
                /* Is random number between 1 and 20 (20% chance) and was person
                 * not recently redirected at the lift queue at summit station? */
                if (random <= 20 && person->going_to != SUMMIT_LIFT_QUEUE) {
                    person->going_to = SUMMIT_LIFT_QUEUE;
                }
                /* Is random number between 21 and 80? (60% chance) */
                else if (random <= 80) {
                    /* Directing person to slope B2 */
                    person->going_to = SLOPE_B2;
                }
                /* Random number is between 81 and 100 (20% chance) */
                else {
                    /* Directing person to slope R2 */
                    person->going_to = SLOPE_R2;
                }
                break;
            }
            case INTERMEDIATE: {
                /* Is random number between 1 and 20? (20% chance) */
                if (random <= 20) {
                    /* Directing person to slope B2 */
                    person->going_to = SLOPE_B2;
                }
                /* Is random number between 21 and 80? (60% chance) */
                else if (random <= 80) {
                    /* Directing person to slope R2 */
                    person->going_to = SLOPE_R2;
                }
                /* Random number is between 81 and 100 (20% chance) */
                else {
                    /* Directing person to slope S1 */
                    person->going_to = SLOPE_S1;
                }
                break;
            }
            case ADVANCED: {
                /* Is random number between 1 and 10? (10% chance) */
                if (random <= 10) {
                    /* Directing person to slope B2 */
                    person->going_to = SLOPE_B2;
                }
                /* Is random number between 11 and 40? (30% chance) */
                else if (random <= 40) {
                    /* Directing person to slope R2 */
                    person->going_to = SLOPE_R2;
                }
                /* Random number is between 41 and 100 (60% chance) */
                else {
                    /* Directing person to slope S1 */
                    person->going_to = SLOPE_S1;
                }
                break;
            }
            default: {}
        }
    }
}


/*  destroy_station():
 *  Nullifies the association to the upward gondola and downward gondola,
 *  destroys the list of people and frees the memory of the given station.
 *      params: reference to station
 *      return: none
 */
void destroy_station(Station* station) {
    /* Is the given station reference valid? */
    if (station) {
        /* Breaking association to the station gondolas */
        station->upward_gondola = NULL;
        station->downward_gondola = NULL;
        /* Destroying list for people at the station */
        destroy_list(station->people_at_station);
        /* Freeing memory of the given station */
        free(station);
    }
}
