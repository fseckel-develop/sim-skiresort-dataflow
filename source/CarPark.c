#include "CarPark.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_car_park():
 *  Creates a car park associated with the given station and the given clock.
 *      params: reference to station, reference to clock
 *      return: reference to car park (or NULL)
 */
CarPark* create_car_park(Station* entry, Clock* clock) {
    /* Are the given station reference and clock reference valid? */
    if (entry && clock) {
        /* Allocating memory for a CarPark struct */
        CarPark* car_park = calloc(1, sizeof(CarPark));
        /* Has the memory allocation been unsuccessful? */
        if (!car_park) {
            fprintf(stderr, "Error in create_car_park: failed to allocate memory for car park\n");
            return NULL;
        }
        /* Associating hotel with the given station */
        car_park->entry = entry;
        /* Associating hotel with the given clock */
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
    /* Is the given car park reference valid and is the car park not full? */
    if (car_park && car_park->car_count < 50) {
        /* Pulling random integer between 1 and 100, uniformly distributed */
        const int random = random_int_in_range(1, 100);
        /* Is random number between 0 and an upper bound dependent on the current
         * arrival density, being 0 at lowest density and 10 at highest density.
         * (seemed to be considerably good rate of car arrivals while testing) */
        if (random <= 10 * arrival_density(car_park->clock->time)) {
            int i = 0;
            /* Creating a new car with passengers */
            Car* arriving_car = create_car(car_park->clock);
            /* Planning shared departure time among passengers */
            plan_departure_time(arriving_car);
            /* Determining the first empty parking spot of the car park */
            while (car_park->parked_cars[i]) {
                i++;
            }
            /* Parking the new car at the found empty spot */
            car_park->parked_cars[i] = arriving_car;
            /* Incrementing the car count */
            car_park->car_count++;
            /* Repeating until new car is empty: */
            while (!car_is_empty(car_park->parked_cars[i])) {
                /* Extracting the next new skier from the car */
                Person* new_skier = leave_car(car_park->parked_cars[i]);
                /* Moving new skier to the entry station */
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
    /* Is the given car reference valid and is the car not empty? */
    if (car && car->passenger_count != 0) {
        Time shared_departure_time;
        int average_departure_s = 0;
        /* Preparing traversal of cars passenger list */
        const Node* current = car->passengers->front;
        while (current) {
            /* Adding up the departure time (in seconds) of current passenger */
            average_departure_s += s(current->person->departure_time);
            /* Progressing list traversal */
            current = current->next;
        }
        /* Calculation the average departure time (in seconds) over all passengers */
        average_departure_s = average_departure_s / car->passenger_count;
        /* Converting the shared departure time into the hh:mm:ss format */
        shared_departure_time = t(0,0,average_departure_s);
        /* Restarting traversal of cars passenger list */
        current = car->passengers->front;
        while (current) {
            /* Setting shared departure time for the current passenger */
            current->person->departure_time = shared_departure_time;
            /* Progressing list traversal */
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
    /* Is the given car park reference valid? */
    if (car_park) {
        /* Preparing traversal of entry stations person list */
        const Node* current = car_park->entry->people_at_station->front;
        while (current) {
            /* Does the current person want to return to the car park? */
            if (current->person->going_to == CAR_PARK) {
                /* Saving reference to current person */
                Person* leaving_person = current->person;
                /* Progressing list traversal for safe element removal */
                current = current->next;
                /* Removing returning passenger from entry stations person list */
                remove_person(car_park->entry->people_at_station, leaving_person);
                /* Adding returning passenger to its associated car */
                enter_own_car(car_park, leaving_person);
                continue;
            }
            /* Progressing list traversal */
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
    /* Are the given car park reference and person reference valid? */
    if (car_park && person) {
        int i;
        /* Repeating until person entered their own car: */
        for (i = 0; i < 50; i++) {
            /* Is the parking spot occupied and is the parked car associated with the person? */
            if (car_park->parked_cars[i] && car_park->parked_cars[i]->id == person->car_id) {
                /* Adding returned passenger to the car passenger list */
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
    /* Is the given car park reference valid and is the car park not empty? */
    if (car_park && 0 < car_park->car_count) {
        int i;
        /* Repeating for each parking spot in the car park: */
        for (i = 0; i < 50; i++) {
            /* Is there a full car at the parking spot? */
            if (car_is_full(car_park->parked_cars[i])) {
                /* Removing car from the parking spot */
                destroy_car(car_park->parked_cars[i]);
                /* Nullifying the parking spot */
                car_park->parked_cars[i] = NULL;
                /* Decrementing the car count */
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
    /* Is the given car park reference valid? */
    if (car_park) {
        int i;
        /* Breaking association with entry station */
        car_park->entry = NULL;
        /* Breaking association with clock */
        car_park->clock = NULL;
        /* Destroying any left car at the car park */
        for (i = 0; i < 50; i++) {
            destroy_car(car_park->parked_cars[i]);
        }
        /* Freeing memory of the given car park */
        free(car_park);
    }
}
