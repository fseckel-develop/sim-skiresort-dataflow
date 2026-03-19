#include "Hotel.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_hotel():
 *  Creates a hotel associated with the given station and the given clock.
 *  The number of residing guest is log_normal randomized between 0 and 60.
 *  The hotel rooms are filled according to the number of guests.
 *      params: reference to station, reference to clock
 *      return: reference to hotel (or NULL)
 */
Hotel* create_hotel(Station* entry, Clock* clock) {
    /* Are the given station reference and clock reference valid? */
    if (entry && clock) {
        /* Allocating memory for a Hotel struct */
        Hotel* hotel = calloc(1, sizeof(Hotel));
        /* Has the memory allocation been unsuccessful? */
        if (!hotel) {
            fprintf(stderr, "Error in create_hotel: failed to allocate memory for hotel\n");
            return NULL;
        }
        /* Associating hotel with the given station */
        hotel->entry = entry;
        /* Associating hotel with the given clock */
        hotel->clock = clock;
        /* Setting initial guest count, favoring higher reservation counts being realized with
         * a negative log-normal distribution with minimum at -60 and mean at -40 (sigma = 1.0) */
        hotel->reservations = (int) -random_log_normal_with_max(-60, -40, 1.0, 0);
        /* Creating a list for present hotel guests */
        hotel->present_guests = create_list();
        /* Has the list creation been unsuccessful? */
        if (!hotel->present_guests) {
            fprintf(stderr, "Error in create_hotel: failed to create list for guests\n");
            /* Destroying hotel cause of incorrect resources */
            destroy_hotel(hotel);
            return NULL;
        }
        /* Filling hotel with guests */
        fill_hotel(hotel);
        return hotel;
    }
    return NULL;
}


/*  fill_hotel():
 *  Creates persons provided with the given hotel clock and
 *  appends them to the guest list of the given hotel.
 *      params: reference to hotel (or NULL)
 *      return: None
 */
void fill_hotel(const Hotel* hotel) {
    /* Is the given hotel reference valid? */
    if (hotel) {
        int i;
        /* Repeating for every guest of the hotel: */
        for (i = 0; i < hotel->reservations; i++) {
            /* Creating new person as guest of the hotel */
            Person* new_guest = create_person(HOTEL, hotel->clock);
            /* Adding new guest to hotels guest list */
            append_list(hotel->present_guests, new_guest);
        }
    }
}


/*  walk_to_resort():
 *  Determines the first well-rested present guest of the given hotel, updates
 *  its arrival time and departure time and moves it to the associated station.
 *  Probability of this is dependent on the current arrival density and lies
 *  between 0% and 10% for each time step of the main clock.
 *      params: reference to hotel
 *      return: None
 */
void guests_walk_to_ski(const Hotel* hotel) {
    if (hotel && !list_is_empty(hotel->present_guests)) {
        if (arrival_event_occurs(hotel->clock->time)) {
            const Node* current = hotel->present_guests->front;
            while (current) {
                /* Has the current guest finished resting? */
                if (activity_finished(current->person)) {
                    /* Saving reference of current guest */
                    Person* new_skier = current->person;
                    /* Removing current guest from hotels guest list */
                    remove_person(hotel->present_guests, new_skier);
                    /* Setting arrival time for the new skier */
                    new_skier->arrival_time = hotel->clock->time;
                    /* Setting departure time for the new skier */
                    set_departure_time(new_skier);
                    /* Moving new skier to the entry station */
                    go_to_station(hotel->entry, new_skier);
                    /* Stopping loop so only the first well-rested guest is leaving */
                    return;
                }
                /* Progressing list traversal */
                current = current->next;
            }
        }
    }
}


/*  guests_are_resting():
 *  Processes guests at the given hotel who are still not well-rested.
 *      params: reference to hotel
 *      return: None
 */
void guests_are_resting(const Hotel* hotel) {
    /* Is the given hotel reference valid? */
    if (hotel) {
        /* Preparing traversal of hotels guest list */
        const Node* current = hotel->present_guests->front;
        while (current) {
            /* Is the current guest still resting? */
            if (!activity_finished(current->person)) {
                /* Reducing resting duration for the current guest */
                proceed_activity(current->person);
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  return_to_hotel():
 *  Moves people from the associated station to the given hotel if
 *  it is their next destination. People are set to rest.
 *      params: reference to hotel
 *      return: None
 */
void return_to_hotel(const Hotel* hotel) {
    /* Is the given hotel reference valid? */
    if (hotel) {
        /* Preparing traversal of entry stations person list */
        const Node* current = hotel->entry->people_at_station->front;
        while (current) {
            /* Does the current person want to return to the hotel? */
            if (current->person->going_to == HOTEL) {
                /* Saving reference to current person */
                Person* returning_guest = current->person;
                /* Progressing list traversal for safe element removal */
                current = current->next;
                /* Removing returning guest from entry stations person list */
                remove_person(hotel->entry->people_at_station, returning_guest);
                /* Setting time to rest for returning guest */
                set_time_to_rest(returning_guest);
                /* Adding returning guest to hotels guest list */
                append_list(hotel->present_guests, returning_guest);
                continue;
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  set_time_to_rest():
 *  Sets a random minimal duration for a given person to stay at
 *  the hotel. Durations between 60 and 120 minutes are possible.
 *      params: reference to person
 *      return: None
 */
void set_time_to_rest(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Setting the persons activity duration between 60 min and 120 min, uniformly distributed */
        person->activity_duration = t(00, random_int_in_range(60, 120), 00);
    }
}


/*  destroy_hotel():
 *  Nullifies the association to the station and clock, destroys
 *  the guest list and frees the memory of the given hotel.
 *      params: reference to hotel
 *      return: None
 */
void destroy_hotel(Hotel* hotel) {
    /* Is the given person reference valid? */
    if (hotel) {
        /* Breaking association with entry station */
        hotel->entry = NULL;
        /* Breaking association with clock */
        hotel->clock = NULL;
        /* Destroying list of hotel guests */
        destroy_list(hotel->present_guests);
        /* Freeing memory of the given hotel */
        free(hotel);
    }
}
