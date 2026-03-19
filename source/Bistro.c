#include "Bistro.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_bistro():
 *  Creates a bistro associated with the given station as its entry point.
 *      params: reference to station
 *      return: reference to bistro (or NULL)
 */
Bistro* create_bistro(Station* entry) {
    if (entry) {
        Bistro* bistro = calloc(1, sizeof(Bistro));
        if (!bistro) {
            fprintf(stderr, "Error in create_bistro: failed to allocate memory for bistro\n");
            return NULL;
        }
        bistro->entry = entry;
        bistro->customers = create_list();
        if (!bistro->customers) {
            fprintf(stderr, "Error in create_bistro: failed to create list for customers\n");
            destroy_bistro(bistro);
            return NULL;
        }
        return bistro;
    }
    return NULL;
}


/*  welcome_customers():
 *  Moves people from the associated station to the bistro during opening hours if their
 *  next destination is the bistro. For each person a stay duration at the bistro is set.
 *  If the bistro is closed, the people are redirected to a different destination.
 *      params: reference to bistro
 *      return: none
 */
void welcome_customers(const Bistro* bistro) {
    if (bistro) {
        const Node* current = bistro->entry->people_at_station->front;
        while (current) {
            if (current->person->going_to == BISTRO) {
                if (bistro_is_open(current->person->clock->time)) {
                    Person* new_customer = current->person;
                    current = current->next;
                    remove_person(bistro->entry->people_at_station, new_customer);
                    set_time_to_eat(new_customer);
                    append_list(bistro->customers, new_customer);
                }
                else {
                    bistro->entry->direct_person(current->person);
                }
                continue;
            }
            current = current->next;
        }
    }
}


/*  bistro_is_open():
 *  Checks if the bistro is open at a given time.
 *  Opening hours are from 9 am to 9 pm.
 *      params: time
 *      return: TRUE or FALSE
 */
Boolean bistro_is_open(const Time time) {
    return s(t(9,00,00)) <= s(time) && s(time) <= s(t(21,00,00));
}


/*  set_time_to_eat():
 *  Sets a random duration for a given person to stay at the bistro.
 *  Durations between 2 and 15 minutes are possible.
 *      params: reference to person
 *      return: none
 */
void set_time_to_eat(Person* person) {
    if (person) {
        person->activity_duration = t(00,random_int_in_range(2, 15),00);
    }
}


/*  customers_are_eating():
 *  Processes customers who are still eating at the given bistro, reducing their personal time to
 *  finish their meal, and moves those back to the associated station who have finished eating.
 *      params: reference to bistro
 *      return: none
 */
void customers_are_eating(const Bistro* bistro) {
    if (bistro) {
        const Node* current = bistro->customers->front;
        while (current) {
            proceed_activity(current->person);
            /* Has the current customer finished eating? */
            if (activity_finished(current->person)) {
                Person* finished_customer = current->person;
                current = current->next;
                remove_person(bistro->customers, finished_customer);
                go_to_station(bistro->entry, finished_customer);
                continue;
            }
            current = current->next;
        }
    }
}


/*  destroy_bistro():
 *  Nullifies the association to the station, destroys the
 *  customer list and frees the memory of the given bistro.
 *      params: reference to bistro
 *      return: none
 */
void destroy_bistro(Bistro* bistro) {
    if (bistro) {
        destroy_list(bistro->customers);
        free(bistro);
    }
}
