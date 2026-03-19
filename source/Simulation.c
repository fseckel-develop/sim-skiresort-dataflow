#include "Simulation.h"
#include "Probability.h"
#include "Input.h"
#include <stdlib.h>
#include <stdio.h>


/*  initiate_simulation():
 *  Creates a simulation by setting up the clock with a start time
 *  as well as creating the structure for the simulated ski resort.
 *      params: none
 *      return: reference to simulation (or NULL)
 */
Simulation* initiate_simulation(void) {
    /* Allocating memory for a Simulation struct */
    Simulation* simulation = calloc(1, sizeof(Simulation));
    /* Has the memory allocation been unsuccessful? */
    if (!simulation) {
        fprintf(stderr, "Error in initiate_simulation: failed to allocate memory for simulation\n");
        return NULL;
    }
    /* Initializing simulation clock with 8:40 am as the starting time */
    simulation->clock = init_clock(t(8,40,00));
    /* Creating the simulated resort */
    simulation->resort = create_resort(simulation->clock);
    return simulation;
}


/*  run_simulation():
 *  Initiates the simulation, enables random number generation and non-blocking key inputs,
 *  and runs the main loop until 10:20 pm. Each time the loop runs, key presses are checked
 *  for controlling the simulated flow of time. If the clock is not stopped, all parts of
 *  the resort are progressing, the state is printed to the console, and time is incremented.
 *  At 10:00 pm left people are cleared from the resort. After the main loop, disabling of
 *  non-blocking key inputs takes places, and the simulation is terminated.
 *      params: none
 *      return: none
 */
void run_simulation(void) {
    /* Initiating of a simulation */
    Simulation* simulation = initiate_simulation();

    /* Loop to ensure the correct setup of the simulation in case that initializing resources
     * failed at one point. It was assumed that unpredictable memory allocation failure while
     * setting up and linking resources would be the main error source in this program.
     * Repeating until valid simulation data structure has been created */
    while (!simulation || !simulation->clock || !simulation->resort) {
        fprintf(stderr, "Error in run_simulation: setup of simulation failed...\n");
        /* Terminating simulation cause of incorrect resources */
        terminate_simulation(simulation);
        /* Another try of initiating new simulation */
        simulation = initiate_simulation();
    }

    /* Enabling random number generation */
    enable_random();
    /* Enabling non-blocking terminal mode */
    enable_non_blocking_terminal_mode();
    /* Repeating until the clock reaches 10:20 pm:*/
    while (s(simulation->clock->time) <= s(t(22,20,00))) {
        /* Is key being pressed on the keyboard? */
        if (key_is_pressed()) {
            /* Reading character from stdin */
            const int key = getchar();
            /* Has the "T" key been pressed and does clock currently run at Normal speed? */
            if ((key == 'T' || key == 't') && simulation->clock->speed == NORMAL) {
                /* Setting clock to Turbo Speed */
                set_turbo_speed(simulation->clock);
            }
            /* Has the "S" key been pressed and does clock currently run at Turbo speed? */
            else if ((key == 'S' || key == 's') && simulation->clock->speed == TURBO) {
                /* Setting clock to Normal Speed */
                set_normal_speed(simulation->clock);
            }
            /* Has the "R" key been pressed and is the clock not currently running? */
            else if ((key == 'R' || key == 'r') && simulation->clock->speed == STOPPED) {
                /* Resetting clock to its last speed */
                set_last_speed(simulation->clock);
            }
            /* Has the "P" key been pressed and is the clock currently running? */
            else if ((key == 'P' || key == 'p') && simulation->clock->speed != STOPPED) {
                /* Stopping clock */
                stop_clock(simulation->clock);
                /* Printing state with updated key control prompt */
                print_resort_state(simulation->resort);
            }
            /* Was the "A" key pressed? */
            else if (key == 'A' || key == 'a') {
                /* Leaving the while-loop to terminate the simulation */
                break;
            }
        }
        /* Is the clock currently running? */
        if (simulation->clock->speed != STOPPED) {
            /* Progressing all parts of the simulated resort */
            progressing_arrivals(simulation->resort);
            progressing_lift_exits(simulation->resort);
            progressing_ski_slopes(simulation->resort);
            progressing_bistro(simulation->resort);
            progressing_lift_queues(simulation->resort);
            progressing_departures(simulation->resort);
            move_lift(simulation->resort->lift);
            /* Has the time reached closing hours of the resort at 10:00 pm? */
            if (s(t(22,00,00)) <= s(simulation->clock->time)) {
                /* Removing all people still located at the resort */
                clean_up_resort(simulation->resort);
            }
            /* Printing the current state of the resort with graphical elements */
            print_resort_state(simulation->resort);
            /* Progressing the simulated time */
            tick_tack(simulation->clock);
        }
    }
    /* Disabling non-blocking terminal mode */
    /* Terminating the simulation */
    disable_non_blocking_terminal_mode();
    terminate_simulation(simulation);
}


/*  terminate_simulation():
 *  Destroys the simulated ski resort and the clock
 *  before freeing the memory of the given simulation.
 *      params: reference to simulation
 *      return: none
 */
void terminate_simulation(Simulation* simulation) {
    /* Is the given simulation reference valid? */
    if (simulation) {
        /* Destroying the simulated resort */
        destroy_resort(simulation->resort);
        /* Destroying the main clock */
        destroy_clock(simulation->clock);
        /* Freeing memory of the given simulation */
        free(simulation);
    }
}
