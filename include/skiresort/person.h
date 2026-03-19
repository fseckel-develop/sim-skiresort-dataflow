#ifndef PERSON_H
#define PERSON_H
#include <skiresort/clock.h>
#include <skiresort/domain.h>


/*  Attributes of Person:
 *   - reference to main clock
 *   - person id number
 *   - id number of persons car
 *   - time of arrival
 *   - planned time for departure
 *   - time to finish activity (eating/skiing/resting)
 *   - skill level
 *   - identifier of origin (busstop/carpark/hotel)
 *   - identifier id of next destination
 */
typedef struct Person {
    Clock* clock;
    int id;
    int car_id;
    int paid_entries;
    Time arrival_time;
    Time departure_time;
    Time activity_duration;
    SkillLevel skill_level;
    Position origin;
    Position going_to;
} Person;


Person* create_person(Position, Clock*);
void set_skill_level(Person*);
void set_departure_time(Person*);
void proceed_activity(Person*);
Boolean activity_finished(const Person*);
double arrival_density(Time);
Boolean arrival_event_occurs(Time);
void destroy_person(Person*);


#endif /* PERSON_H */
