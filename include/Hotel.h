#ifndef HOTEL_H
#define HOTEL_H
#include "Station.h"


/*  Attributes of Hotel:
 *   - reference to resort station at the hotel
 *   - reference to the main clock
 *   - number of reservations
 *   - reference to list of present guests
 */
typedef struct Hotel {
    Station* entry;
    Clock* clock;
    int reservations;
    List* present_guests;
} Hotel;


Hotel* create_hotel(Station*, Clock*);
void fill_hotel(const Hotel*);
void walk_to_resort(const Hotel*);
void guests_are_resting(const Hotel*);
void return_to_hotel(const Hotel*);
void set_time_to_rest(Person*);
void destroy_hotel(Hotel*);


#endif /* HOTEL_H */
