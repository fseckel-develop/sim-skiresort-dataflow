#ifndef CAR_H
#define CAR_H
#include <skiresort/list.h>


/*  Attributes of Car:
 *   - car id number
 *   - number of passengers
 *   - reference to list of passengers
 */
typedef struct Car {
    int id;
    int passenger_count;
    List* passengers;
} Car;


Car* create_car(Clock*);
void fill_car(const Car*, Clock*);
Person* leave_car(const Car*);
Boolean car_is_full(const Car*);
Boolean car_is_empty(const Car*);
void destroy_car(Car*);


#endif /* CAR_H */
