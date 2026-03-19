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
    Gondola* gondola = calloc(1, sizeof(Gondola));
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
    if (gondola && person && gondola->passenger_count < GONDOLA_CAPACITY) {
        int i;
        for (i = 0; i < GONDOLA_CAPACITY; i++) {
            if (!gondola->seat[i]) {
                gondola->seat[i] = person;
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
    if (gondola && person && 0 < gondola->passenger_count) {
        int i;
        for (i = 0; i < GONDOLA_CAPACITY; i++) {
            if (gondola->seat[i] == person) {
                gondola->seat[i] = NULL;
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
    if (gondola) {
        int i = 0;
        for (i = 0; i < GONDOLA_CAPACITY; i++) {
            destroy_person(gondola->seat[i]);
        }
        gondola->follower = NULL;
        free(gondola);
    }
}
