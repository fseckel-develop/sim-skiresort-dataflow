#include "LiftQueue.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_lift_queue():
 *  Creates a lift queue with the given id and associates it with the given station.
 *      params: resort part id, reference to station
 *      return: reference to lift queue (or NULL)
 */
LiftQueue* create_lift_queue(const Position id, Station* entry) {
    /* Is the given station reference valid? */
    if (entry) {
        /* Allocating memory for a LiftQueue struct */
        LiftQueue* lift_queue = calloc(1, sizeof(LiftQueue));
        /* Has the memory allocation been unsuccessful? */
        if (!lift_queue) {
            fprintf(stderr, "Error in create_lift_queue: failed to allocate memory for lift queue\n");
            return NULL;
        }
        /* Setting the lift queues id to the given id */
        lift_queue->id = id;
        /* Associating lift queue with the given station */
        lift_queue->entry = entry;
        /* Creating a queue for waiting lift passengers */
        lift_queue->queue = create_queue();
        /* Has the queue creation been unsuccessful? */
        if (!lift_queue->queue) {
            fprintf(stderr, "Error in create_lift_queue: failed to create queue for skiers\n");
            /* Destroying lift queue cause of incorrect resources */
            destroy_lift_queue(lift_queue);
            return NULL;
        }
        /* Setting the gondola currently located at the lift queue */
        set_entry_gondola(lift_queue);
        return lift_queue;
    }
    return NULL;
}


/*  set_entry_gondola():
 *  Depending on the id of the given lift queue its associated entry
 *  gondola is set to one of the gondolas of the associated station.
 *      params: reference to lift queue
 *      return: none
 */
void set_entry_gondola(LiftQueue* lift_queue) {
    /* Is the given lift queue reference valid? */
    if (lift_queue) {
        /* Is the lift queue located at valley station or at middle station going upward? */
        if (lift_queue->id == VALLEY_LIFT_QUEUE || lift_queue->id == MIDDLE_LIFT_QUEUE_UP) {
            /* Setting entry gondola to the upward gondola of the associated station */
            lift_queue->entry_gondola = lift_queue->entry->upward_gondola;
        }
        /* Is the lift queue located at summit station or at middle station going downward? */
        else if (lift_queue->id == SUMMIT_LIFT_QUEUE || lift_queue->id == MIDDLE_LIFT_QUEUE_DOWN) {
            /* Setting entry gondola to the downward gondola of the associated station */
            lift_queue->entry_gondola = lift_queue->entry->downward_gondola;
        }
    }
}


/*  enqueue_people():
 *  Moves people from the associated station to the given lift queue if it
 *  is their next destination and if the lift is open. Otherwise, people
 *  are redirected at the associated station of the given lift queue.
 *      params: reference to lift queue
 *      return: none
 */
void enqueue_people(LiftQueue* lift_queue) {
    /* Is the given lift queue reference valid? */
    if (lift_queue) {
        /* Preparing traversal of entry stations person list */
        const Node* current = lift_queue->entry->people_at_station->front;
        while (current) {
            /* Does the current person want to enter this lift queue? */
            if (current->person->going_to == lift_queue->id) {
                /* Is the lift currently open? */
                if (lift_is_open(current->person->clock->time)) {
                    /* Saving reference to current person */
                    Person* waiting_passenger = current->person;
                    /* Progressing list traversal for safe element removal */
                    current = current->next;
                    /* Removing new waiting passenger from entry stations person list */
                    remove_person(lift_queue->entry->people_at_station, waiting_passenger);
                    /* Controlling if new waiting passenger has a lift ticket */
                    ticket_control(lift_queue, waiting_passenger);
                    /* Putting the new waiting passenger into the lift queue */
                    enqueue(lift_queue->queue, waiting_passenger);
                }
                /* Lift is currently closed */
                else {
                    /* Redirecting current person at entry station */
                    lift_queue->entry->direct_person(current->person);
                }
                continue;
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  lift_is_open():
 *  Checks if people can enter the lift at a given time. To ensure that lift
 *  passengers will not get stuck on the lift and can reach any lift exit, the
 *  last entry time is set to 7:51 pm, 9 minutes before the lift stops moving.
 *      params: time
 *      return: TRUE or FALSE
 */
Boolean lift_is_open(const Time time) {
    return s(t(9,0,0)) <= s(time) && s(time) <= s(t(19,51,0));
}


/*  ticket_control():
 *  Checks the ticket ownership of the given person at the given lift queue. In case
 *  the person has no paid lift entries left, they are buying a ten-ticket. People who
 *  already bought a ten-ticket, will not buy a day-ticket afterward, assuming that it
 *  would not be economical to buy a day-ticket when most time has passed until departure.
 *  For each sold ticket the ticket counter of the given lift queue is incremented.
 *      params: reference to lift queue, reference to person
 *      return: none
 */
void ticket_control(LiftQueue* lift_queue, Person* passenger) {
    /* Are the given lift queue reference and person reference valid? */
    if (lift_queue && passenger) {
        /* Will the passenger purchase his first ticket now? */
        if (passenger->paid_entries == NO_FIRST_PURCHASE) {
            /* Which ticket type does the passenger choose for today? */
            switch (choose_ticket_type(passenger)) {
                /* Passenger chose ten-ticket */
                case TEN_TICKET: {
                    /* Setting the passengers paid lift entries to 10 */
                    passenger->paid_entries = TEN_TICKET;
                    /* Incrementing the count of sold ten-tickets at the lift queue */
                    lift_queue->sold_ten_tickets++;
                    break;
                }
                /* Passenger chose day-ticket */
                case DAY_TICKET: {
                    /* Setting the passengers paid lift entries to "infinite" (-1) */
                    passenger->paid_entries = DAY_TICKET;
                    /* Incrementing the count of sold day-tickets at the lift queue */
                    lift_queue->sold_day_tickets++;
                    break;
                }
                default: {}
            }
        }
        /* Does the passenger still have paid lift entries left? */
        else if (passenger->paid_entries > 0) {
            /* Decrementing left number of paid lift entries */
            passenger->paid_entries--;
        }
        /* Does the passenger have no lift entries left?
         * (only possible after purchase of ten-ticket) */
        else if (passenger->paid_entries == 0) {
            /* Setting the passengers paid lift entries to 10 */
            passenger->paid_entries = TEN_TICKET;
            /* Incrementing the count of sold ten-tickets at the lift queue */
            lift_queue->sold_ten_tickets++;
        }
    }
}


/*  choose_ticket_type():
 *  Determines the ticket type the given person will purchase at a lift queue.
 *  In accordance with the average departure time, the likeliness for choosing
 *  a day-ticket over a ten-ticket increases with the skill level, assuming that
 *  more experienced skiers tend to get the most of their day at the ski resort
 *  and know their moneys worth is well spent. Apart from that, the likeliness
 *  for a day-ticket decreases with the time of day.
 *      params: reference to person
 *      return: ticket type
 */
TicketType choose_ticket_type(const Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Pulling random integer between 0 and 100, uniformly distributed */
        const int random = random_int_in_range(1, 100);
        /* Which skill level does the person have? */
        switch (person->skill_level) {
            case FRIGHTENED: {
                /* Is it morning (before 12 pm)? */
                if (person->clock->time.h < 12) {
                    /* Person purchase ten-ticket with 70% chance */
                    return random <= 70 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 12 pm and 4 pm)? */
                if (person->clock->time.h < 16) {
                    /* Person purchase ten-ticket with 80% chance */
                    return random <= 80 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 4 pm and 8 pm)? */
                if (person->clock->time.h < 20) {
                    /* Person purchase ten-ticket with 90% chance */
                    return random <= 90 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            case BEGINNER: {
                /* Is it morning (before 12 pm)? */
                if (person->clock->time.h < 12) {
                    /* Person chooses ten-ticket with 50% chance */
                    return random <= 50 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 12 pm and 4 pm)? */
                if (person->clock->time.h < 16) {
                    /* Person chooses ten-ticket with 65% chance */
                    return random <= 65 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 4 pm and 8 pm)? */
                if (person->clock->time.h < 20) {
                    /* Person chooses ten-ticket with 80% chance */
                    return random <= 80 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            case INTERMEDIATE: {
                /* Is it morning (before 12 pm)? */
                if (person->clock->time.h < 12) {
                    /* Person chooses ten-ticket with 30% chance */
                    return random <= 30 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 12 pm and 4 pm)? */
                if (person->clock->time.h < 16) {
                    /* Person chooses ten-ticket with 50% chance */
                    return random <= 50 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 4 pm and 8 pm)? */
                if (person->clock->time.h < 20) {
                    /* Person chooses ten-ticket with 70% chance */
                    return random <= 70 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            case ADVANCED: {
                /* Is it morning (before 12 pm)? */
                if (person->clock->time.h < 12) {
                    /* Person chooses ten-ticket with 10% chance */
                    return random <= 10 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 12 pm and 4 pm)? */
                if (person->clock->time.h < 16) {
                    /* Person chooses ten-ticket with 35% chance */
                    return random <= 35 ? TEN_TICKET : DAY_TICKET;
                }
                /* Is it afternoon (between 4 pm and 8 pm)? */
                if (person->clock->time.h < 20) {
                    /* Person chooses ten-ticket with 60% chance */
                    return random <= 60 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            default: {}
        }
    }
    return 0;
}


/*  enter_lift():
 *  Moves people inside the given lift queue onto the current entry gondola if the
 *  entry gondola has a free seat - and the lift has not closed since enqueuing.
 *  Otherwise, people are redirected at the associated station of the given lift queue.
 *  When going to be seated, the desired lift exit is set for the new passenger.
 *      params: reference to lift queue
 *      return: none
 */
void enter_lift(const LiftQueue* lift_queue) {
    /* Is the given lift queue reference valid? */
    if (lift_queue) {
        int i;
        for (i = 0; i < GONDOLA_CAPACITY; i++) {
            if (!queue_is_empty(lift_queue->queue) && !lift_queue->entry_gondola->seat[i]) {
                /* Extracting new passenger from the front of the queue */
                Person* new_passenger = dequeue(lift_queue->queue);
                /* Is the lift currently still open? */
                if (lift_is_open(new_passenger->clock->time)) {
                    /* New passenger choose station to exit the lift */
                    choose_exit_station(new_passenger);
                    /* New passenger is seated on the empty gondola seat */
                    enter_gondola(lift_queue->entry_gondola, new_passenger);
                }
                /* Lift has closed since person arrived at lift queue */
                else {
                    /* Redirecting current person at entry station */
                    go_to_station(lift_queue->entry, new_passenger);
                }
            }
        }
    }
}


/*  choose_exit_station():
 *  Sets the desired lift exit depending on the skill level of the given person.
 *  Coming from the valley station, the likeliness to exit at the middle station
 *  decreases with the skill level. Coming from the summit station, the likeliness
 *  to exit at the middle station increases with the skill level. The function
 *  also ensures the mandatory exiting at the summit station and valley station.
 *      params: reference to person
 *      return: none
 */
void choose_exit_station(Person* passenger) {
    /* Is the given person reference valid? */
    if (passenger) {
        /* Pulling random integer between 0 and 100, uniformly distributed */
        const int random = random_int_in_range(1, 100);
        /* At which lift queue is the person entering the lift? */
        switch (passenger->going_to) {
            case VALLEY_LIFT_QUEUE: {
                /* Which skill level does the person have? */
                switch (passenger->skill_level) {
                    case FRIGHTENED: {
                        /* Choosing middle station to exit with 60% chance, otherwise summit station */
                        passenger->going_to = random <= 60 ? MIDDLE_STATION : SUMMIT_STATION;
                        break;
                    }
                    case BEGINNER: {
                        /* Choosing middle station to exit with 40% chance, otherwise summit station */
                        passenger->going_to = random <= 40 ? MIDDLE_STATION : SUMMIT_STATION;
                        break;
                    }
                    case INTERMEDIATE: {
                        /* Choosing middle station to exit with 20% chance, otherwise summit station */
                        passenger->going_to = random <= 20 ? MIDDLE_STATION : SUMMIT_STATION;
                        break;
                    }
                    case ADVANCED: {
                        /* Person chooses summit station to exit */
                        passenger->going_to = SUMMIT_STATION;
                        break;
                    }
                    default: {}
                }
                break;
            }
            case MIDDLE_LIFT_QUEUE_UP: {
                /* Person has to exit at summit station */
                passenger->going_to = SUMMIT_STATION;
                break;
            }
            case MIDDLE_LIFT_QUEUE_DOWN: {
                /* Person has to exit at valley station */
                passenger->going_to = VALLEY_STATION;
                break;
            }
            case SUMMIT_LIFT_QUEUE: {
                /* Which skill level does the person have? */
                switch (passenger->skill_level) {
                    case FRIGHTENED: {
                        /* Choosing middle station to exit with 40% chance, otherwise valley station */
                        passenger->going_to = random <= 40 ? MIDDLE_STATION : VALLEY_STATION;
                        break;
                    }
                    case BEGINNER: {
                        /* Choosing middle station to exit with 80% chance, otherwise valley station */
                        passenger->going_to = random <= 80 ? MIDDLE_STATION : VALLEY_STATION;
                        break;
                    }
                    /* Default case for intermediate and advanced skiers, actually not necessary
                     * cause these skill levels are not directed to lift queue at summit station */
                    default: {
                        /* Person chooses middle station to exit */
                        passenger->going_to = MIDDLE_STATION;
                    }
                }
                break;
            }
            default: {}
        }
    }
}


/*  destroy_lift_queue():
 *  Nullifies the association to the station and entry gondola,
 *  destroys the queue and frees the memory of the lift queue.
 *      params: reference to car park
 *      return: none
 */
void destroy_lift_queue(LiftQueue* lift_queue) {
    /* Is the given lift queue reference valid? */
    if (lift_queue) {
        /* Breaking association with entry station */
        lift_queue->entry = NULL;
        /* Breaking association with entry gondola */
        lift_queue->entry_gondola = NULL;
        /* Destroying queue for waiting lift passengers */
        destroy_queue(lift_queue->queue);
        /* Freeing memory of the given lift queue */
        free(lift_queue);
    }
}
