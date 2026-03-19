#ifndef GONDOLA_H
#define GONDOLA_H
#define GONDOLA_CAPACITY 4
#include "Person.h"


/*  Attributes of Gondola:
 *   - number of passengers
 *   - array of references to sitting persons
 *   - reference to the following gondola
 */
typedef struct Gondola {
    int passenger_count;
    Person* seat[GONDOLA_CAPACITY];
    struct Gondola* follower;
} Gondola;


Gondola* create_gondola(void);
void enter_gondola(Gondola*, Person*);
void exit_gondola(Gondola*, const Person*);
void destroy_gondola(Gondola*);


#endif /* GONDOLA_H */
