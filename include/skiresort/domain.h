#ifndef DOMAIN_H
#define DOMAIN_H


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


#endif /* DOMAIN_H */
