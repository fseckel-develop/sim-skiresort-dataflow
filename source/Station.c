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
    if (lift) {
        Station* station = calloc(1, sizeof(Station));
        if (!station) {
            fprintf(stderr, "Error in create_station: failed to allocate memory for station\n");
            return NULL;
        }
        station->id = id;
        station->people_at_station = create_list();
        if (!station->people_at_station) {
            fprintf(stderr, "Error in create_station: failed to create list for people\n");
            destroy_station(station);
            return NULL;
        }
        switch (station->id) {
            case VALLEY_STATION: {
                station->direct_person = direct_people_at_valley_station;
                break;
            }
            case MIDDLE_STATION: {
                station->direct_person = direct_people_at_middle_station;
                break;
            }
            case SUMMIT_STATION: {
                station->direct_person = direct_people_at_summit_station;
                break;
            }
            default: {}
        }
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
    if (station && lift) {
        switch (station->id) {
            case VALLEY_STATION: {
                station->upward_gondola = lift->valley_entry;
                station->downward_gondola = lift->valley_exit;
                break;
            }
            case MIDDLE_STATION: {
                station->upward_gondola = lift->middle_upward;
                station->downward_gondola = lift->middle_downward;
                break;
            }
            case SUMMIT_STATION: {
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
    if (station) {
        int i;
        for (i = 0; i < GONDOLA_CAPACITY; i++) {
            Person* passenger = station->upward_gondola->seat[i];
            if (passenger && passenger->going_to == station->id) {
                exit_gondola(station->upward_gondola, passenger);
                go_to_station(station, passenger);
            }
            passenger = station->downward_gondola->seat[i];
            if (passenger && passenger->going_to == station->id) {
                exit_gondola(station->downward_gondola, passenger);
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
    if (station && person) {
        append_list(station->people_at_station, person);
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
    if (person) {
        if (s(person->clock->time) < s(person->departure_time) && person->going_to != VALLEY_LIFT_QUEUE) {
            person->going_to = VALLEY_LIFT_QUEUE;
        }
        else {
            person->going_to = person->origin;
        }
    }
}


/*  direct_people_at_middle_station():
 *  Randomly sets the next destination of the given person to the bistro, one of the middle
 *  lift queues or one of the slopes depending on the skill level of the person. If the person
 *  is coming back from one of these destinations (due to a finished bistro visit or cause of
 *  the lift already being closed) these destinations are ignored in the random choice.
 *      ,---------------,---------------,---------------,---------------,---------------,
 *      | Destination:  | Frightened:   | Beginner:     | Intermediate: | Advanced:     |
 *      | Bistro        | 40%           | 20%           | 20%           | 20%           |
 *      | Queue Up      | 10%           | 20%           | 30%           | 30%           |
 *      | Queue Down    | 30%           | 10%           |  0%           |  0%           |
 *      | Slope B1      | 20%           | 40%           | 20%           | 10%           |
 *      | Slope R1      |  0%           | 10%           | 30%           | 40%           |
 *      '---------------'---------------'---------------'---------------'---------------'
 *      params: reference to person
 *      return: none
 */
void direct_people_at_middle_station(Person* person) {
    if (person) {
        const int random = random_int_in_range(1, 100);
        switch (person->skill_level) {
            case FRIGHTENED: {
                if (random <= 40 && person->going_to != BISTRO) {
                    person->going_to = BISTRO;
                }
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                else if (random <= 80 && person->going_to != MIDDLE_LIFT_QUEUE_DOWN) {
                    person->going_to = MIDDLE_LIFT_QUEUE_DOWN;
                }
                else {
                    person->going_to = SLOPE_B1;
                }
                break;
            }
            case BEGINNER: {
                if (random <= 20 && person->going_to != BISTRO) {
                    person->going_to = BISTRO;
                }
                else if (random <= 40 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_DOWN) {
                    person->going_to = MIDDLE_LIFT_QUEUE_DOWN;
                }
                else if (random <= 90) {
                    person->going_to = SLOPE_B1;
                }
                else {
                    person->going_to = SLOPE_R1;
                }
                break;
            }
            case INTERMEDIATE: {
                if (random <= 20 && person->going_to != BISTRO) {
                    person->going_to = BISTRO;
                }
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                else if (random <= 70) {
                    person->going_to = SLOPE_B1;
                }
                else {
                    person->going_to = SLOPE_R1;
                }
                break;
            }
            case ADVANCED: {
                if (random <= 20 && person->going_to != BISTRO) {
                    person->going_to = BISTRO;
                }
                else if (random <= 50 && person->going_to != MIDDLE_LIFT_QUEUE_UP) {
                    person->going_to = MIDDLE_LIFT_QUEUE_UP;
                }
                else if (random <= 60) {
                    person->going_to = SLOPE_B1;
                }
                else {
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
 *      ,---------------,---------------,---------------,---------------,---------------,
 *      | Destination:  | Frightened:   | Beginner:     | Intermediate: | Advanced:     |
 *      | Queue         | 80%           | 20%           |  0%           |  0%           |
 *      | Slope B2      | 20%           | 60%           | 20%           | 10%           |
 *      | Slope R2      |  0%           | 20%           | 60%           | 30%           |
 *      | Slope S1      |  0%           |  0%           | 20%           | 60%           |
 *      '---------------'---------------'---------------'---------------'---------------'
 *      params: reference to person
 *      return: none
 */
void direct_people_at_summit_station(Person* person) {
    if (person) {
        const int random = random_int_in_range(1, 100);
        switch (person->skill_level) {
            case FRIGHTENED: {
                if (random <= 80 && person->going_to != SUMMIT_LIFT_QUEUE) {
                    person->going_to = SUMMIT_LIFT_QUEUE;
                }
                else {
                    person->going_to = SLOPE_B2;
                }
                break;
            }
            case BEGINNER: {
                if (random <= 20 && person->going_to != SUMMIT_LIFT_QUEUE) {
                    person->going_to = SUMMIT_LIFT_QUEUE;
                }
                else if (random <= 80) {
                    person->going_to = SLOPE_B2;
                }
                else {
                    person->going_to = SLOPE_R2;
                }
                break;
            }
            case INTERMEDIATE: {
                if (random <= 20) {
                    person->going_to = SLOPE_B2;
                }
                else if (random <= 80) {
                    person->going_to = SLOPE_R2;
                }
                else {
                    person->going_to = SLOPE_S1;
                }
                break;
            }
            case ADVANCED: {
                if (random <= 10) {
                    person->going_to = SLOPE_B2;
                }
                else if (random <= 40) {
                    person->going_to = SLOPE_R2;
                }
                else {
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
    if (station) {
        station->upward_gondola = NULL;
        station->downward_gondola = NULL;
        destroy_list(station->people_at_station);
        free(station);
    }
}
