#ifndef BUS_H
#define BUS_H
#define BUS_CAPACITY 50
#include "List.h"


/*  Attributes of Bus:
 *   - number of passengers
 *   - reference to list of passengers
 */
typedef struct Bus {
    int passenger_count;
    List* passengers;
} Bus;


Bus* create_bus(Clock*);
int init_passenger_count(Time);
void fill_bus(const Bus*, Clock*);
Person* leave_bus(const Bus*);
Boolean bus_is_full(const Bus*);
Boolean bus_is_empty(const Bus*);
void destroy_bus(Bus*);


#endif /* BUS_H */
