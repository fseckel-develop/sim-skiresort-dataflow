#include <skiresort/slope.h>
#include <skiresort/probability.h>
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
    if (entry && finish) {
        Slope* slope = calloc(1, sizeof(Slope));
        if (!slope) {
            fprintf(stderr, "Error in create_slope: failed to allocate memory for slope\n");
            return NULL;
        }
        slope->id = id;
        slope->entry = entry;
        slope->finish = finish;
        slope->fastest = fastest;
        slope->average = average;
        slope->skiers = create_list();
        if (!slope->skiers) {
            fprintf(stderr, "Error in create_slope: failed to create list for skiers\n");
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
    if (slope) {
        const Node* current = slope->entry->people_at_station->front;
        while (current) {
            if (current->person->going_to == slope->id) {
                Person* new_skier = current->person;
                current = current->next;
                remove_person(slope->entry->people_at_station, new_skier);
                set_time_to_ski(slope, new_skier);
                append_list(slope->skiers, new_skier);
                slope->total_entries++;
                continue;
            }
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
    if (slope && person) {
        const int ski_minutes = (int) random_log_normal(slope->fastest, slope->average, 1.0);
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
    if (slope) {
        const Node* current = slope->skiers->front;
        while (current) {
            proceed_activity(current->person);
            if (activity_finished(current->person)) {
                Person* finished_skier = current->person;
                current = current->next;
                remove_person(slope->skiers, finished_skier);
                go_to_station(slope->finish, finished_skier);
                continue;
            }
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
    if (slope && drop_off) {
        const Node* current = slope->skiers->front;
        while (current) {
            Person* late_skier = current->person;
            current = current->next;
            remove_person(slope->skiers, late_skier);
            late_skier->going_to = late_skier->origin;
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
    if (slope) {
        slope->entry = NULL;
        slope->finish = NULL;
        destroy_list(slope->skiers);
        free(slope);
    }
}
