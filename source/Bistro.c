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
    /* Is the given station reference valid? */
    if (entry) {
        Bistro* bistro = calloc(1, sizeof(Bistro));
        if (!bistro) {
            fprintf(stderr, "Error in create_bistro: failed to allocate memory for bistro\n");
            return NULL;
        }
        /* Associating bistro with the given station */
        bistro->entry = entry;
        /* Creating a list for bistro customers */
        bistro->customers = create_list();
        /* Has the list creation been unsuccessful? */
        if (!bistro->customers) {
            fprintf(stderr, "Error in create_bistro: failed to create list for customers\n");
            /* Destroying bistro cause of incorrect resources */
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
    /* Is the given bistro reference valid? */
    if (bistro) {
        /* Preparing traversal of entry stations person list */
        const Node* current = bistro->entry->people_at_station->front;
        while (current) {
            /* Does the current person want to enter the bistro? */
            if (current->person->going_to == BISTRO) {
                /* Is the bistro currently open? */
                if (bistro_is_open(current->person->clock->time)) {
                    /* Saving reference to current person */
                    Person* new_customer = current->person;
                    /* Progressing list traversal for safe element removal */
                    current = current->next;
                    /* Removing new customer from entry stations person list */
                    remove_person(bistro->entry->people_at_station, new_customer);
                    /* Setting time to eat for new customer */
                    set_time_to_eat(new_customer);
                    /* Adding new customer to bistros customer list */
                    append_list(bistro->customers, new_customer);
                }
                /* Bistro is currently closed */
                else {
                    /* Redirecting current person at entry station */
                    bistro->entry->direct_person(current->person);
                }
                continue;
            }
            /* Progressing list traversal */
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
    /* Is the given person reference valid? */
    if (person) {
        /* Setting the persons activity duration between 2 min and 15 min, uniformly distributed */
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
    /* Is the given bistro reference valid? */
    if (bistro) {
        /* Preparing traversal of bistros customer list */
        const Node* current = bistro->customers->front;
        while (current) {
            /* Reducing eating duration for the current customer */
            proceed_activity(current->person);
            /* Has the current customer finished eating? */
            if (activity_finished(current->person)) {
                /* Saving reference of current customer */
                Person* finished_customer = current->person;
                /* Progressing list traversal for safe element removal */
                current = current->next;
                /* Removing finished customer from bistros customer list */
                remove_person(bistro->customers, finished_customer);
                /* Moving finished customer to the entry station */
                go_to_station(bistro->entry, finished_customer);
                continue;
            }
            /* Progressing list traversal */
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
    /* Is the given bistro reference valid? */
    if (bistro) {
        /* Breaking association with entry station */
        bistro->entry = NULL;
        /* Destroying list of customers */
        destroy_list(bistro->customers);
        /* Freeing memory of the given bistro */
        free(bistro);
    }
}
