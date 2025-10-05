#include "Person.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_person():
 *  Creates a person, associating it with the given clock and setting the given resort
 *  part as its origin. The skill level is initialized and a departure time is estimated.
 *  Static person count is incremented with every created person.
 *      params: resort part id, reference to clock
 *      return: reference to person (or NULL)
 */
Person* create_person(const Position origin, Clock* clock) {
    /* Declaring static person count for the entire run of the program */
    static int person_count = 0;
    /* Are the given clock reference and the given origin position valid? */
    if (clock && (origin == CAR_PARK || origin == BUS_STOP || origin == HOTEL)) {
        /* Allocating memory for a Person struct */
        Person* person = malloc(sizeof(Person));
        /* Has the memory allocation been unsuccessful? */
        if (!person) {
            fprintf(stderr, "Error in create_person: failed to allocate memory for person\n");
            return NULL;
        }
        /* Associating person with the given clock */
        person->clock = clock;
        /* Giving person a unique ID while incrementing person count */
        person->id = person_count++;
        /* Person is not associated with an existing car initially */
        person->car_id = -1;
        /* Person has not purchased any ticket initially  */
        person->paid_entries = NO_FIRST_PURCHASE;
        /* Setting the persons arrival time */
        person->arrival_time = clock->time;
        /* Setting the persons origin to the given position */
        person->origin = origin;
        /* Person has not been directed initially */
        person->going_to = UNDEFINED;
        /* Setting the persons skill level */
        set_skill_level(person);
        /* Setting the persons departure time */
        set_departure_time(person);
        return person;
    }
    return NULL;
}


/*  Randomly chooses a skill level for the given person based on the following
 *  distribution: 10% Frightened, 40% Beginner, 30% Intermediate, 20% Advanced.
 *      params: reference to person
 *      return: none
 */
void set_skill_level(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Pulling random integer between 1 and 100, uniformly distributed */
        const int random = random_int_in_range(1, 100);
        /* Is random number between 1 and 10? (10% chance) */
        if (random <= 10) {
            /* Setting persons skill level to Frightened */
            person->skill_level = FRIGHTENED;
        }
        /* Is random number between 11 and 50? (40% chance) */
        else if (random <= 50) {
            /* Setting persons skill level to Beginner */
            person->skill_level = BEGINNER;
        }
        /* Is random number between 51 and 80? (30% chance) */
        else if (random <= 80) {
            /* Setting persons skill level to Intermediate */
            person->skill_level = INTERMEDIATE;
        }
        /* Random number is between 81 and 100 (20% chance) */
        else {
            /* Setting persons skill level to Advanced */
            person->skill_level = ADVANCED;
        }
    }
}


/*  set_departure_time():
 *  Determines a random departure time for the given person. The average stay increases
 *  with the skill level of the person, assuming that more experienced skiers tend to
 *  spend more time at the resort. It is calculated by adding a random duration,
 *  normally distributed with an average of 1h, to a fixed base duration.
 *  Total average stay durations: Frightened 2h, Beginner 3h, Intermediate 4h, Advanced 5h.
 *      params: reference to person
 *      return: none
 */
void set_departure_time(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Pulling random integer between 0 and 120, normally distributed */
        const int random = (int) random_normal_with_bounds(60, 20, 0, 120);
        /* Which skill level does the person have? */
        switch (person->skill_level) {
            case FRIGHTENED: {
                /* Setting departure time to 1h + random minutes */
                person->departure_time = add(person->arrival_time, t(1, random, 0));
                break;
            }
            case BEGINNER: {
                /* Setting departure time to 2h + random minutes */
                person->departure_time = add(person->arrival_time, t(2, random, 0));
                break;
            }
            case INTERMEDIATE: {
                /* Setting departure time to 3h + random minutes */
                person->departure_time = add(person->arrival_time, t(3, random, 0));
                break;
            }
            case ADVANCED: {
                /* Setting departure time to 4h + random minutes */
                person->departure_time = add(person->arrival_time, t(4, random, 0));
                break;
            }
            default: {}
        }
    }
}


/*  proceeds_activity():
 *  Decreases the time the given person needs to finish an activity (eating/skiing/resting).
 *      params: reference to person
 *      return: none
 */
void proceed_activity(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Decrementing the activity duration by 10s */
        person->activity_duration.s -= 10;
        /* Has the next full minute begun? */
        if (person->activity_duration.s == -10) {
            /* Updating the activity duration minutes */
            person->activity_duration.min--;
            /* Correcting the activity duration seconds */
            person->activity_duration.s = 50;
        }
    }
}


/*  activity_finished():
 *  Checks if the given person has finished their activity (eating/skiing/resting).
 *      params: reference to person
 *      return: TRUE or FALSE
 */
Boolean activity_finished(const Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Has the activity duration reached 00:00? */
        return person->activity_duration.min <= 0 && person->activity_duration.s <= 0;
    }
    return FALSE;
}


/*  arrival_density():
 *  Determines the density of arrivals depending on the given time. Density increases linearly
 *  from 8:50 am to 11:00 am, stays constant until 2 pm, and decreases linearly from 2:00 pm
 *  until 5:00 pm. This leads to a significant decrease of visitor density approximately between
 *  5:36 pm and 8:36 pm on average since the mean stay duration over all skill levels set by
 *  set_departure_time() is 10% * 2h + 40% * 3h + 30% * 4h + 20% * 5h = 3.6h = 3h + 36min.
 *      params: time
 *      return: arrival density (double between 0.0 and 1.0)
 */
double arrival_density(const Time time) {
    /* Is it before 8:50 am? */
    if (time.h <= 8 && time.min < 50) {
        return 0.0;
    }
    /* Is it between 8:50 am and 11:00 am? */
    if (time.h == 8 || (9 <= time.h && time.h < 11)) {
        /* time passed since 8:50 am divided by 2h 10min (time between 8:50 am and 11:00 am) */
        return (double) (s(time) - s(t(8,50,0))) / (double) s(t(2,10,0));
    }
    /* Is it between 11:00 am and 2:00 pm? */
    if (11 <= time.h && time.h < 14) {
        return 1.0;
    }
    /* Is it between 2:00 pm and 5:00 pm? */
    if (14 <= time.h && time.h < 17) {
        /* time left until 5:00 pm divided by 3h (time between 2:00 pm and 5:00 pm) */
        return (double) (s(t(17,0,0)) - s(time)) / (double) s(t(3,0,0));
    }
    return 0.0;
}


/*  destroy_person():
 *  Nullifies the association to the clock and frees the memory of the given bistro.
 *      params: reference to person
 *      return: none
 */
void destroy_person(Person* person) {
    /* Is the given person reference valid? */
    if (person) {
        /* Breaking association with clock */
        person->clock = NULL;
        /* Freeing memory of the given person */
        free(person);
    }
}
