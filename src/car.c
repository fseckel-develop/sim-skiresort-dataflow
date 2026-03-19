#include <skiresort/car.h>
#include <skiresort/probability.h>
#include <stdlib.h>
#include <stdio.h>


/*  create_car():
 *  Creates a car and fills it with 1 to 5 passengers providing them with the given clock.
 *      params: reference to clock
 *      return: reference to car (or NULL)
 */
Car* create_car(Clock* clock) {
    static int car_count = 0;
    if (clock) {
        Car* car = calloc(1, sizeof(Car));
        if (!car) {
            fprintf(stderr, "Error in create_car: failed to allocate memory for car\n");
            return NULL;
        }
        car->id = car_count++;
        car->passenger_count = random_int_in_range(1, 5);
        car->passengers = create_list();
        if (!car->passengers) {
            fprintf(stderr, "Error in create_car: failed to create list for passengers\n");
            destroy_car(car);
            return NULL;
        }
        fill_car(car, clock);
        return car;
    }
    return NULL;
}


/*  fill_car():
 *  Creates persons provided with the given clock and
 *  appends them to the passenger list of the given car.
 *  The car id is associated with each of the passengers.
 *      params: reference to car, reference to clock
 *      return: none
 */
void fill_car(const Car* car, Clock* clock) {
    if (car && clock) {
        int i;
        for (i = 0; i < car->passenger_count; i++) {
            Person* new_passenger = create_person(CAR_PARK, clock);
            new_passenger->car_id = car->id;
            append_list(car->passengers, new_passenger);
        }
    }
}


/*  leave_car():
 *  Gets the next passenger of the given car and returns it.
 *      params: reference to car
 *      return: reference to person (or NULL)
 */
Person* leave_car(const Car* car) {
    if (car && !list_is_empty(car->passengers)) {
        Person* new_skier = car->passengers->front->person;
        remove_person(car->passengers, new_skier);
        return new_skier;
    }
    return NULL;
}


/*  car_is_full():
 *  Checks of the initial passenger count for the given car is reached again.
 *      params: reference to car
 *      return: TRUE or FALSE
 */
Boolean car_is_full(const Car* car) {
    if (car) {
        return car->passengers->size == car->passenger_count;
    }
    return FALSE;
}


/*  car_is_empty():
 *  Checks if the passenger list of the given car is empty.
 *      params: reference to car
 *      return: TRUE or FALSE
 */
Boolean car_is_empty(const Car* car) {
    if (car) {
        return list_is_empty(car->passengers);
    }
    return FALSE;
}


/*  destroy_car():
 *  Destroys the passenger list and frees the memory of the given car.
 *      params: reference to car
 *      return: none
 */
void destroy_car(Car* car) {
    if (car) {
        destroy_list(car->passengers);
        free(car);
    }
}
