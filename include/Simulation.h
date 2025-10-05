#ifndef SIMULATION_H
#define SIMULATION_H
#include "Resort.h"


/* Attributes of Simulation:
 *  - reference to the simulated resort
 *  - reference to the main clock
 */
typedef struct Simulation {
    Resort* resort;
    Clock* clock;
} Simulation;


Simulation* initiate_simulation(void);
void run_simulation(void);
void terminate_simulation(Simulation*);


#endif /* SIMULATION_H */
