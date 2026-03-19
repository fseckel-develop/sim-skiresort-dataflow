#ifndef CAR_PARK_H
#define CAR_PARK_H
#define CAR_PARK_CAPACITY 50
#include <skiresort/station.h>
#include <skiresort/car.h>


/*  Attributes of CarPark:
 *   - reference to resort station at the car park
 *   - reference to the main clock
 *   - number of parked cars
 *   - array of references to parked cars
 */
typedef struct CarPark {
    Station* entry;
    Clock* clock;
    int car_count;
    Car* parked_cars[CAR_PARK_CAPACITY];
} CarPark;


CarPark* create_car_park(Station*, Clock*);
void car_is_arriving(CarPark*);
void plan_departure_time(const Car*);
void return_to_car_park(const CarPark*);
void enter_own_car(const CarPark*, Person*);
void cars_are_departing(CarPark*);
void destroy_car_park(CarPark*);


#endif /* CAR_PARK_H */
