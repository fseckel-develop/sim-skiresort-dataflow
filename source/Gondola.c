#include "Gondola.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_gondola():
 *  Creates a gondola and initializes its attributes.
 *  Static gondola count is incremented with every created gondola.
 *      params: none
 *      return: reference to gondola (or NULL)
 */
Gondola* create_gondola(void) {
    /* Allocating memory for a Gondola struct */
    Gondola* gondola = calloc(1, sizeof(Gondola));
    /* Has the memory allocation been unsuccessful? */
    if (!gondola) {
        fprintf(stderr, "Error in create_gondola: failed to allocate memory for gondola...\n");
        return NULL;
    }
    return gondola;
}


/*  enter_gondola():
 *  Places the given person on the first free seat of the
 *  given gondola while incrementing its passenger count.
 *      params: reference to gondola, reference to person
 *      return: none
 */
void enter_gondola(Gondola* gondola, Person* person) {
    /* Is given gondola reference valid and is at least one seat empty? */
    if (gondola && person && gondola->passenger_count < 4) {
        int i;
        /* Repeating until person has found empty seat: */
        for (i = 0; i < 4; i++) {
            /* Is current gondola seat empty? */
            if (!gondola->seat[i]) {
                /* Associating given person with gondola seat */
                gondola->seat[i] = person;
                /* Incrementing passenger count of gondola */
                gondola->passenger_count++;
                break;
            }
        }
    }
}


/*  exit_gondola():
 *  Removes the given person from the given gondola if it is
 *  one of the passengers. Passenger count is decremented.
 *      params: reference to gondola, reference to person
 *      return: none
 */
void exit_gondola(Gondola* gondola, const Person* person) {
    /* Are the given gondola reference and person reference valid and is the gondola not empty? */
    if (gondola && person && 0 < gondola->passenger_count) {
        int i;
        /* Repeating until seat of given person is found: */
        for (i = 0; i < 4; i++) {
            /* Is given person associated with current gondola seat? */
            if (gondola->seat[i] == person) {
                /* Nullifying seat to break association with person */
                gondola->seat[i] = NULL;
                /* Decrementing passenger count of gondola */
                gondola->passenger_count--;
                break;
            }
        }
    }
}


/*  destroy_gondola():
 *  Destroys left persons on the gondola seats, nullifies the association
 *  to the following gondola and frees the memory of the given gondola.
 *      params: reference to gondola
 *      return: none
 */
void destroy_gondola(Gondola* gondola) {
    /* Is the given gondola reference valid? */
    if (gondola) {
        int i = 0;
        /* Destroying any person still associated to a gondola seat */
        for (i = 0; i < 4; i++) {
            destroy_person(gondola->seat[i]);
        }
        /* Breaking association to following gondola */
        gondola->follower = NULL;
        /* Freeing memory of the given gondola */
        free(gondola);
    }
}
