#include "Slope.h"
#include "Probability.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_slope():
 *  Creates a slope with the given id, associates it with the given stations at entry and
 *  finish and sets the given fastest and average time for a ski run down this slope.
 *      params: resort part id, reference to start station, reference to finish station,
 *              number of minutes for fastest run, number of minutes for average run
 *      return: reference to slope (or NULL)
 */
Slope* create_slope(const Position id, Station* entry, Station* finish, const int fastest, const int average) {
    /* Are the given station references valid? */
    if (entry && finish) {
        /* Allocating memory for a Slope struct */
        Slope* slope = malloc(sizeof(Slope));
        /* Has the memory allocation been unsuccessful? */
        if (!slope) {
            fprintf(stderr, "Error in create_slope: failed to allocate memory for slope\n");
            return NULL;
        }
        /* Setting the slope id to the given id */
        slope->id = id;
        /* Associating slope with the given stations */
        slope->entry = entry;
        slope->finish = finish;
        /* Setting the characteristic skiing times for the slope */
        slope->fastest = fastest;
        slope->average = average;
        /* Initializing the slopes entry count */
        slope->total_entries = 0;
        /* Creating a list for skiers on the slope */
        slope->skiers = create_list();
        /* Has the list creation been unsuccessful? */
        if (!slope->skiers) {
            fprintf(stderr, "Error in create_slope: failed to create list for skiers\n");
            /* Destroying slope cause of incorrect resources */
            destroy_slope(slope);
            return NULL;
        }
        return slope;
    }
    return NULL;
}


/*  let_people_enter():
 *  Moves people from the associated start station to the given slope if
 *  it is their next destination. Each new skier gets assigned a time to
 *  finish the slope and the slope entry counter is incremented.
 *      params: reference to slope
 *      return: none
 */
void let_people_enter(Slope* slope) {
    /* Is the given slope reference valid? */
    if (slope) {
        /* Preparing traversal of entry stations person list */
        const Node* current = slope->entry->people_at_station->front;
        while (current) {
            /* Does the current person want to enter the slope? */
            if (current->person->going_to == slope->id) {
                /* Saving reference to current person */
                Person* new_skier = current->person;
                /* Progressing list traversal for safe element removal */
                current = current->next;
                /* Removing new skier from entry stations person list */
                remove_person(slope->entry->people_at_station, new_skier);
                /* Setting time to ski for new skier */
                set_time_to_ski(slope, new_skier);
                /* Adding new skier to slopes skier list */
                append_list(slope->skiers, new_skier);
                /* Incrementing the slopes entry count */
                slope->total_entries++;
                continue;
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  set_time_to_ski():
 *  Sets a random duration for a given person to ski down the given slope. Durations
 *  are following a log-normal distribution with the fastest skiing time of the slope
 *  as the minimum value and the average skiing time of the slope as the mean.
 *      params: reference to slope, reference to person
 *      return: none
 */
void set_time_to_ski(const Slope* slope, Person* person) {
    /* Are the given slope reference and person reference valid? */
    if (slope && person) {
        /* Computing random log-normally distributed skiing duration respecting the slopes statistics */
        const int ski_minutes = (int) random_log_normal(slope->fastest, slope->average, 1.0);
        /* Setting the skiing duration as the persons activity duration */
        person->activity_duration = t(00,ski_minutes,00);
    }
}


/*  people_are_skiing():
 *  Processes skiers who are still skiing down the given slope and moves
 *  those to the associated finish station who have finished their ski run.
 *      params: reference to slope, reference to person
 *      return: none
 */
void people_are_skiing(const Slope* slope) {
    /* Is the given slope reference valid? */
    if (slope) {
        /* Preparing traversal of slopes skier list */
        const Node* current = slope->skiers->front;
        while (current) {
            /* Reducing skiing duration for the current skier */
            proceed_activity(current->person);
            /* Has the current skier finished the slope? */
            if (activity_finished(current->person)) {
                /* Saving reference of current skier */
                Person* finished_skier = current->person;
                /* Progressing list traversal for safe element removal */
                current = current->next;
                /* Removing finished skier from slopes skier list */
                remove_person(slope->skiers, finished_skier);
                /* Moving finished skier to the finish station */
                go_to_station(slope->finish, finished_skier);
                continue;
            }
            /* Progressing list traversal */
            current = current->next;
        }
    }
}


/*  collect_skiers_with_snowmobile():
 *  Moves all skiers left on the given slope to the given drop-off station.
 *      params: reference to slope, reference to station
 *      return: none
 */
void collect_skiers_with_snowmobile(const Slope* slope, const Station* drop_off) {
    /* Are the given slope reference and station reference valid? */
    if (slope && drop_off) {
        /* Preparing traversal of slopes skier list */
        const Node* current = slope->skiers->front;
        while (current) {
            /* Saving reference of current skier */
            Person* late_skier = current->person;
            /* Progressing list traversal for safe element removal */
            current = current->next;
            /* Removing late skier from slopes skier list */
            remove_person(slope->skiers, late_skier);
            /* Directing late skier to the resort part they arrived at */
            late_skier->going_to = late_skier->origin;
            /* Moving late skier to the drop-off station */
            go_to_station(drop_off, late_skier);
        }
    }
}


/*  destroy_slope():
 *  Nullifies the association to the start station and finish station,
 *  destroys the list of skiers and frees the memory of the given slope.
 *      params: reference to slope
 *      return: none
 */
void destroy_slope(Slope* slope) {
    /* Is the given slope reference valid? */
    if (slope) {
        /* Breaking associating with entry and finish station */
        slope->entry = NULL;
        slope->finish = NULL;
        /* Destroying list of skiers */
        destroy_list(slope->skiers);
        /* Freeing memory of the given slope */
        free(slope);
    }
}
