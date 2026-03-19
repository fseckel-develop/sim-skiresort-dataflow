#include "Car.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_car():
 *  Creates a car and fills it with 1 to 5 passengers providing them with the given clock.
 *      params: reference to clock
 *      return: reference to car (or NULL)
 */
Car* create_car(Clock* clock) {
    /* Declaring static car count for the entire run of the program */
    static int car_count = 0;
    /* Is the given clock reference valid? */
    if (clock) {
        /* Allocating memory for a Car struct */
        Car* car = calloc(1, sizeof(Car));
        /* Has the memory allocation been unsuccessful? */
        if (!car) {
            fprintf(stderr, "Error in create_car: failed to allocate memory for car\n");
            return NULL;
        }
        /* Giving car a unique ID while incrementing car count */
        car->id = car_count++;
        /* Setting random passenger count between 1 and 5, uniformly distributed */
        car->passenger_count = random_int_in_range(1, 5);
        /* Creating a list for car passengers */
        car->passengers = create_list();
        /* Has the list creation been unsuccessful? */
        if (!car->passengers) {
            fprintf(stderr, "Error in create_car: failed to create list for passengers\n");
            /* Destroying car cause of incorrect resources */
            destroy_car(car);
            return NULL;
        }
        /* Filling car with passengers */
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
    /* Are the given car reference and clock reference valid? */
    if (car && clock) {
        int i;
        /* Repeating for every passenger of the car: */
        for (i = 0; i < car->passenger_count; i++) {
            /* Creating new person as passenger of the car */
            Person* new_passenger = create_person(CAR_PARK, clock);
            /* Associating new passenger with car */
            new_passenger->car_id = car->id;
            /* Adding new passenger to cars passenger list */
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
    /* Is the given car reference valid and is the car not empty? */
    if (car && !list_is_empty(car->passengers)) {
        /* Saving reference to first passenger of list for return */
        Person* new_skier = car->passengers->front->person;
        /* Removing passenger from cars passenger list */
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
    /* Is the given car reference valid? */
    if (car) {
        /* Has the car its initial passenger count? */
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
    /* Is the given car reference valid? */
    if (car) {
        /* Is the cars passenger list empty? */
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
    /* Is the given car reference valid? */
    if (car) {
        /* Destroying list of passengers */
        destroy_list(car->passengers);
        /* Freeing memory of the given car */
        free(car);
    }
}
