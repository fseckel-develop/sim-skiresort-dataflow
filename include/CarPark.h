#ifndef CARPARK_H
#define CARPARK_H
#include "Station.h"
#include "Car.h"


/*  Attributes of CarPark:
 *   - reference to resort station at the car park
 *   - reference to the main clock
 *   - number of parked cars
 *   - array of 50 references to parked cars
 */
typedef struct CarPark {
    Station* entry;
    Clock* clock;
    int car_count;
    Car* parked_cars[50];
} CarPark;


CarPark* create_car_park(Station*, Clock*);
void car_is_arriving(CarPark*);
void plan_departure_time(const Car*);
void return_to_car_park(const CarPark*);
void enter_own_car(const CarPark*, Person*);
void cars_are_departing(CarPark*);
void destroy_car_park(CarPark*);


#endif /* CARPARK_H */
