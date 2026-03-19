#include "CarPark.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_car_park():
 *  Creates a car park associated with the given station and the given clock.
 *      params: reference to station, reference to clock
 *      return: reference to car park (or NULL)
 */
CarPark* create_car_park(Station* entry, Clock* clock) {
    if (entry && clock) {
        CarPark* car_park = calloc(1, sizeof(CarPark));
        if (!car_park) {
            fprintf(stderr, "Error in create_car_park: failed to allocate memory for car park\n");
            return NULL;
        }
        car_park->entry = entry;
        car_park->clock = clock;
        return car_park;
    }
    return NULL;
}


/*  car_is_arriving():
 *  Creates a car and parks it at the first free spot of the given car park.
 *  Probability of arriving is dependent on the current arrival density and
 *  lies between 0% and 10% for each time step of the main clock. All its
 *  passengers are moved to the station associated with the car park.
 *  The number of parked cars is incremented.
 *      params: reference to car park
 *      return: none
 */
void car_is_arriving(CarPark* car_park) {
    if (car_park && car_park->car_count < CAR_PARK_CAPACITY) {
        if (arrival_event_occurs(car_park->clock->time)) {
            int i = 0;
            Car* arriving_car = create_car(car_park->clock);
            plan_departure_time(arriving_car);
            while (car_park->parked_cars[i]) {
                i++;
            }
            car_park->parked_cars[i] = arriving_car;
            car_park->car_count++;
            while (!car_is_empty(car_park->parked_cars[i])) {
                Person* new_skier = leave_car(car_park->parked_cars[i]);
                go_to_station(car_park->entry, new_skier);
            }
        }
    }
}


/*  plan_departure_time():
 *  Determines the average departure time over the passengers in the given car
 *  and sets it as the new shared departure time to favor a collective return.
 *      params: reference to car
 *      return: none
 */
void plan_departure_time(const Car* car) {
    if (car && car->passenger_count != 0) {
        Time shared_departure_time;
        int average_departure_s = 0;
        const Node* current = car->passengers->front;
        while (current) {
            average_departure_s += s(current->person->departure_time);
            current = current->next;
        }
        average_departure_s = average_departure_s / car->passenger_count;
        shared_departure_time = t(0,0,average_departure_s);
        current = car->passengers->front;
        while (current) {
            current->person->departure_time = shared_departure_time;
            current = current->next;
        }
    }
}


/*  return_to_car_park():
 *  Moves people from the associated station to the given car park if
 *  it is their next destination. People are put into their own car.
 *      params: reference to car park
 *      return: none
 */
void return_to_car_park(const CarPark* car_park) {
    if (car_park) {
        const Node* current = car_park->entry->people_at_station->front;
        while (current) {
            if (current->person->going_to == CAR_PARK) {
                Person* leaving_person = current->person;
                current = current->next;
                remove_person(car_park->entry->people_at_station, leaving_person);
                enter_own_car(car_park, leaving_person);
                continue;
            }
            current = current->next;
        }
    }
}


/*  enter_own_car():
 *  Searches the given car park for the car with same id as the car id
 *  of the given person. Once found, the person is moved to this car.
 *      params: reference to car park, reference to person
 *      return: none
 */
void enter_own_car(const CarPark* car_park, Person* person) {
    if (car_park && person) {
        int i;
        for (i = 0; i < CAR_PARK_CAPACITY; i++) {
            /* Is the parking spot occupied and is the parked car associated with the person? */
            if (car_park->parked_cars[i] && car_park->parked_cars[i]->id == person->car_id) {
                append_list(car_park->parked_cars[i]->passengers, person);
                break;
            }
        }
    }
}


/*  cars_are_departing():
 *  Checks every car in the given car park if all its passengers have returned.
 *  In this case, the spot of the car park is emptied and the car destroyed.
 *  The number of parked cars is decremented accordingly.
 *      params: reference to car park
 *      return: none
 */
void cars_are_departing(CarPark* car_park) {
    if (car_park && 0 < car_park->car_count) {
        int i;
        for (i = 0; i < CAR_PARK_CAPACITY; i++) {
            if (car_is_full(car_park->parked_cars[i])) {
                destroy_car(car_park->parked_cars[i]);
                car_park->parked_cars[i] = NULL;
                car_park->car_count--;
            }
        }
    }
}


/*  destroy_car_park():
 *  Nullifies the association to the station and clock, destroys
 *  any left car in the given car park and frees its memory.
 *      params: reference to car park
 *      return: none
 */
void destroy_car_park(CarPark* car_park) {
    if (car_park) {
        int i;
        car_park->entry = NULL;
        car_park->clock = NULL;
        for (i = 0; i < CAR_PARK_CAPACITY; i++) {
            destroy_car(car_park->parked_cars[i]);
        }
        free(car_park);
    }
}
