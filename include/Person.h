#ifndef PERSON_H
#define PERSON_H
#include "Clock.h"


/* Boolean values */
typedef enum Boolean {
    FALSE,
    TRUE
} Boolean;


/* Possible ticket types */
typedef enum TicketType {
    NO_FIRST_PURCHASE = -2,
    DAY_TICKET = -1,
    TEN_TICKET = 10
} TicketType;


/* Possible resort positions */
typedef enum Position {
    UNDEFINED,
    VALLEY_STATION,
    MIDDLE_STATION,
    SUMMIT_STATION,
    VALLEY_LIFT_QUEUE,
    MIDDLE_LIFT_QUEUE_UP,
    MIDDLE_LIFT_QUEUE_DOWN,
    SUMMIT_LIFT_QUEUE,
    SLOPE_B1,
    SLOPE_R1,
    SLOPE_B2,
    SLOPE_R2,
    SLOPE_S1,
    BUS_STOP,
    CAR_PARK,
    HOTEL,
    BISTRO
} Position;


/* Possible skill levels */
typedef enum SkillLevel {
    FRIGHTENED,
    BEGINNER,
    INTERMEDIATE,
    ADVANCED
} SkillLevel;


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
