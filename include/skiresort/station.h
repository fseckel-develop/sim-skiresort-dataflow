#ifndef STATION_H
#define STATION_H
#include <skiresort/lift.h>
#include <skiresort/list.h>


/* Attributes of Station:
 *  - identifier
 *  - reference to lift gondola going upwards
 *  - reference to lift gondola going downwards
 *  - reference to list of people
 *  - reference to function for directing people
 */
typedef struct Station {
    Position id;
    Gondola* upward_gondola;
    Gondola* downward_gondola;
    List* people_at_station;
    void (*direct_person)(Person*);
} Station;


Station* create_station(Position, const Lift*);
void set_gondolas(Station*, const Lift*);
void exit_lift(const Station*);
void go_to_station(const Station*, Person*);
void direct_people_at_valley_station(Person*);
void direct_people_at_middle_station(Person*);
void direct_people_at_summit_station(Person*);
void destroy_station(Station*);


#endif /* STATION_H */
