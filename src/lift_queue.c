#include <skiresort/lift_queue.h>
#include <skiresort/probability.h>
#include <stdlib.h>
#include <stdio.h>


/*  create_lift_queue():
 *  Creates a lift queue with the given id and associates it with the given station.
 *      params: resort part id, reference to station
 *      return: reference to lift queue (or NULL)
 */
LiftQueue* create_lift_queue(const Position id, Station* entry) {
    if (entry) {
        LiftQueue* lift_queue = calloc(1, sizeof(LiftQueue));
        if (!lift_queue) {
            fprintf(stderr, "Error in create_lift_queue: failed to allocate memory for lift queue\n");
            return NULL;
        }
        lift_queue->id = id;
        lift_queue->entry = entry;
        lift_queue->queue = create_queue();
        if (!lift_queue->queue) {
            fprintf(stderr, "Error in create_lift_queue: failed to create queue for skiers\n");
            destroy_lift_queue(lift_queue);
            return NULL;
        }
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
    if (lift_queue) {
        if (lift_queue->id == VALLEY_LIFT_QUEUE || lift_queue->id == MIDDLE_LIFT_QUEUE_UP) {
            lift_queue->entry_gondola = lift_queue->entry->upward_gondola;
        }
        else if (lift_queue->id == SUMMIT_LIFT_QUEUE || lift_queue->id == MIDDLE_LIFT_QUEUE_DOWN) {
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
    if (lift_queue) {
        const Node* current = lift_queue->entry->people_at_station->front;
        while (current) {
            if (current->person->going_to == lift_queue->id) {
                if (lift_is_open(current->person->clock->time)) {
                    Person* waiting_passenger = current->person;
                    current = current->next;
                    remove_person(lift_queue->entry->people_at_station, waiting_passenger);
                    ticket_control(lift_queue, waiting_passenger);
                    enqueue(lift_queue->queue, waiting_passenger);
                }
                else {
                    lift_queue->entry->direct_person(current->person);
                }
                continue;
            }
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
    if (lift_queue && passenger) {
        if (passenger->paid_entries == NO_FIRST_PURCHASE) {
            switch (choose_ticket_type(passenger)) {
                case TEN_TICKET: {
                    passenger->paid_entries = TEN_TICKET;
                    lift_queue->sold_ten_tickets++;
                    break;
                }
                case DAY_TICKET: {
                    passenger->paid_entries = DAY_TICKET;
                    lift_queue->sold_day_tickets++;
                    break;
                }
                default: {}
            }
        }
        else if (passenger->paid_entries > 0) {
            passenger->paid_entries--;
        }
        /* Does the passenger have no lift entries left?
         * (only possible after purchase of ten-ticket) */
        else if (passenger->paid_entries == 0) {
            passenger->paid_entries = TEN_TICKET;
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
 *  for a day-ticket decreases with the time of day.  (TT = Ten-Ticket)
 *      ,----------------,---------------,---------------,---------------,---------------,
 *      | Time:          | FRIGHTENED:   | BEGINNER:     | INTERMEDIATE: | ADVANCED:     |
 *      |  before 12 pm  | 70% TT        | 50% TT        | 30% TT        | 10% TT        |
 *      |  12 pm - 4 pm  | 80% TT        | 65% TT        | 50% TT        | 35% TT        |
 *      |   4 pm - 8 pm  | 90% TT        | 80% TT        | 70% TT        | 60% TT        |
 *      '----------------'---------------'---------------'---------------'---------------'
 *      params: reference to person
 *      return: ticket type
 */
TicketType choose_ticket_type(const Person* person) {
    if (person) {
        const int random = random_int_in_range(1, 100);
        switch (person->skill_level) {
            case FRIGHTENED: {
                if (person->clock->time.h < 12) {
                    return random <= 70 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 16) {
                    return random <= 80 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 20) {
                    return random <= 90 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            case BEGINNER: {
                if (person->clock->time.h < 12) {
                    return random <= 50 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 16) {
                    return random <= 65 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 20) {
                    return random <= 80 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            case INTERMEDIATE: {
                if (person->clock->time.h < 12) {
                    return random <= 30 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 16) {
                    return random <= 50 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 20) {
                    return random <= 70 ? TEN_TICKET : DAY_TICKET;
                }
                break;
            }
            case ADVANCED: {
                if (person->clock->time.h < 12) {
                    return random <= 10 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 16) {
                    return random <= 35 ? TEN_TICKET : DAY_TICKET;
                }
                if (person->clock->time.h < 20) {
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
    if (lift_queue) {
        int i;
        for (i = 0; i < GONDOLA_CAPACITY; i++) {
            if (!queue_is_empty(lift_queue->queue) && !lift_queue->entry_gondola->seat[i]) {
                Person* new_passenger = dequeue(lift_queue->queue);
                if (lift_is_open(new_passenger->clock->time)) {
                    choose_exit_station(new_passenger);
                    enter_gondola(lift_queue->entry_gondola, new_passenger);
                }
                else {
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
 *      ,----------,---------------,---------------,---------------,---------------,
 *      | Entry:   | FRIGHTENED:   | BEGINNER:     | INTERMEDIATE: | ADVANCED:     |
 *      |  valley  | 60% middle    | 40% middle    | 20% middle    | 100% summit   |
 *      |  summit  | 40% middle    | 80% middle    |  (no entry)   |  (no entry)   |
 *      '----------'---------------'---------------'---------------'---------------'
 *      params: reference to person
 *      return: none
 */
void choose_exit_station(Person* passenger) {
    if (passenger) {
        const int random = random_int_in_range(1, 100);
        switch (passenger->going_to) {
            case VALLEY_LIFT_QUEUE: {
                switch (passenger->skill_level) {
                    case FRIGHTENED: {
                        passenger->going_to = random <= 60 ? MIDDLE_STATION : SUMMIT_STATION;
                        break;
                    }
                    case BEGINNER: {
                        passenger->going_to = random <= 40 ? MIDDLE_STATION : SUMMIT_STATION;
                        break;
                    }
                    case INTERMEDIATE: {
                        passenger->going_to = random <= 20 ? MIDDLE_STATION : SUMMIT_STATION;
                        break;
                    }
                    case ADVANCED: {
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
                switch (passenger->skill_level) {
                    case FRIGHTENED: {
                        passenger->going_to = random <= 40 ? MIDDLE_STATION : VALLEY_STATION;
                        break;
                    }
                    case BEGINNER: {
                        passenger->going_to = random <= 80 ? MIDDLE_STATION : VALLEY_STATION;
                        break;
                    }
                    /* Default case for intermediate and advanced skiers, actually not necessary
                     * cause these skill levels are not directed to lift queue at summit station */
                    default: {
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
    if (lift_queue) {
        lift_queue->entry = NULL;
        lift_queue->entry_gondola = NULL;
        destroy_queue(lift_queue->queue);
        free(lift_queue);
    }
}
