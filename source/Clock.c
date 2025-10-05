#include "Clock.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*  init_clock():
 *  Creates a clock and initializes it with the given time at normal speed.
 *      params: time
 *      return: none
 */
Clock* init_clock(const Time time) {
    /* Allocating memory for a Clock struct */
    Clock* clock = malloc(sizeof(Clock));
    /* Has the memory allocation been unsuccessful? */
    if (!clock) {
        fprintf(stderr, "Error in init_clock: failed to allocate memory for clock\n");
        return NULL;
    }
    /* Setting time of the clock to the given time */
    clock->time = time;
    /* Clock initially runs at normal speed */
    clock->speed = NORMAL;
    clock->last_speed = NORMAL;
    return clock;
}


/*  stop_clock():
 *  Saves the current speed of the given clock and stops it from running.
 *      params: reference to clock
 *      return: none
 */
void stop_clock(Clock* clock) {
    /* Is the given clock reference valid? */
    if (clock) {
        /* Saving the current speed of the clock */
        clock->last_speed = clock->speed;
        /* Stopping the clock */
        clock->speed = STOPPED;
    }
}


/*  set_normal_speed():
 *  Sets the speed of the given glock to Normal.
 *      params: reference to clock
 *      return: none
 */
void set_normal_speed(Clock* clock) {
    /* Is the given clock reference valid? */
    if (clock) {
        /* Setting the clocks speed to Normal */
        clock->speed = NORMAL;
    }
}


/*  set_turbo_speed():
 *  Sets the speed of the given glock to Turbo.
 *      params: reference to clock
 *      return: none
 */
void set_turbo_speed(Clock* clock) {
    /* Is the given clock reference valid? */
    if (clock) {
        /* Setting the clocks speed to Turbo */
        clock->speed = TURBO;
    }
}


/*  set_last_speed():
 *  Resets the speed of the given before it was stopped.
 *      params: reference to clock
 *      return: none
 */
void set_last_speed(Clock* clock) {
    /* Is the given clock reference valid and is the clock currently not running? */
    if (clock && clock->speed == STOPPED) {
        /* Setting the clocks speed to the last saved speed */
        clock->speed = clock->last_speed;
    }
}


/*  wait_seconds():
 *  Implements a waiting loop to pause program execution for a specified
 *  number of seconds. Uses the clock() function to measure elapsed CPU time.
 *      params: seconds as floating point
 *      return: none
 */
void wait_seconds(const double seconds) {
    /* Saving the number of clock ticks elapsed since the program started */
    const clock_t start_time = clock();
    /* Calculating an end time by adding clock ticks for each given second to the start time */
    const clock_t end_time = start_time + (clock_t)(seconds * CLOCKS_PER_SEC);
    /* Waiting until the end time has been reached */
    while (clock() < end_time) {}
}


/*  tick_tack():
 *  Increments the time of the given clock by 10 seconds and waits 166667 microseconds
 *  (1/6 of a second) at normal speed to achieve 1 simulated minute per real-time second,
 *  or 16667 microseconds (1/60 of a second) at turbo speed to achieve 10 simulated minutes
 *  per real-time second. Seconds are converted into full minutes and full hours, if necessary.
 *      params: reference to clock
 *      return: none
 */
void tick_tack(Clock* clock) {
    /* Is the given clock reference valid and is the clock running? */
    if (clock && clock->speed != STOPPED) {
        /* Does the clock currently run at normal speed? */
        if (clock->speed == NORMAL) {
            /* Waiting 1/6 of a second = 166667 microseconds */
            wait_seconds(0.166667);
        }
        else if (clock->speed == TURBO) {
            /* Waiting 1/60 of a second = 16667 microseconds */
            wait_seconds(0.016667);
        }
        /* Progressing clock time by 10 seconds */
        clock->time.s += 10;
        /* Has a full minute been reached? */
        if (clock->time.s == 60) {
            /* Resetting number of seconds */
            clock->time.s = 0;
            /* Incrementing number of minutes */
            clock->time.min++;
            /* Has a full hour been reached? */
            if (clock->time.min == 60) {
                /* Resetting number of minutes */
                clock->time.min = 0;
                /* Incrementing number of hours */
                clock->time.h++;
            }
        }
    }
}


/*  destroy_clock():
 *  Frees the memory of the given clock.
 *      params: reference to clock
 *      return: none
 */
void destroy_clock(Clock* clock) {
    /* Is the given clock reference valid? */
    if (clock) {
        /* Freeing memory of the given clock */
        free(clock);
    }
}


/*  t():
 *  Determines a time structure in a valid time format
 *  from the given amount of hours, minutes and seconds.
 *      params: number of hours, minutes and seconds
 *      return: time
 */
Time t(const int h, const int min, const int s) {
    Time time;
    /* Number of full hours = number of hours + number of full 60 minutes */
    time.h = h + (min + s / 60) / 60;
    /* Number of remaining minutes */
    time.min = (min + s / 60) % 60;
    /* Number of remaining seconds */
    time.s = s % 60;
    return time;
}


/*  add():
 *  Adds two times by adding their hours, minutes and seconds before reformatting.
 *      params: first time, second time
 *      return: time
 */
Time add(const Time t1, const Time t2) {
    return t(t1.h + t2.h, t1.min + t2.min, t1.s + t2.s);
}


/*  min():
 *  Determines the number of full minutes from a given time.
 *      params: time
 *      return: number of minutes
 */
int min(const Time time) {
    return 60 * time.h + time.min;
}


/*  s():
 *  Determines the number of seconds from a given time.
 *      params: time
 *      return: number of seconds
 */
int s(const Time time) {
    return 60 * min(time) + time.s;
}


/*  print_time():
 *  Prints a given time to the console, respecting the XX:XX format.
 *      params: time
 *      return: number of seconds
 */
void print_time(const Time time) {
    time.h < 10 ? printf("0%d", time.h) : printf("%2d", time.h);
    time.min % 2 == 0 ? printf(" ") : printf(":");
    time.min < 10 ? printf("0%d", time.min) : printf("%2d", time.min);
}
