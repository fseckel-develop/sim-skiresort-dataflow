#ifndef LIFT_QUEUE_H
#define LIFT_QUEUE_H
#include <skiresort/station.h>
#include <skiresort/queue.h>


/*  Attributes of LiftQueue:
 *   - identifier
 *   - reference to associated resort station
 *   - reference to associated entry gondola
 *   - number of sold ten-tickets
 *   - number of sold day-tickets
 *   - reference to queue of waiting skiers
 */
typedef struct LiftQueue {
    Position id;
    Station* entry;
    Gondola* entry_gondola;
    int sold_ten_tickets;
    int sold_day_tickets;
    Queue* queue;
} LiftQueue;


LiftQueue* create_lift_queue(Position, Station*);
void set_entry_gondola(LiftQueue*);
void enqueue_people(LiftQueue*);
Boolean lift_is_open(Time);
void ticket_control(LiftQueue*, Person*);
TicketType choose_ticket_type(const Person*);
void enter_lift(const LiftQueue*);
void choose_exit_station(Person*);
void destroy_lift_queue(LiftQueue*);


#endif /* LIFT_QUEUE_H */
