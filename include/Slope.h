#ifndef TRACK_H
#define TRACK_H
#include "Station.h"


/* Attributes of Slope:
 *  - identifier
 *  - references to the stations at start and finish
 *  - number of minutes for fastest and average ski run
 *  - number of entries
 *  - reference to list of skiers
 */
typedef struct Slope {
    Position id;
    Station* entry;
    Station* finish;
    int fastest;
    int average;
    int total_entries;
    List* skiers;
} Slope;


Slope* create_slope(Position, Station*, Station*, int, int);
void let_people_enter(Slope*);
void set_time_to_ski(const Slope*, Person*);
void people_are_skiing(const Slope*);
void collect_skiers_with_snowmobile(const Slope*, const Station*);
void destroy_slope(Slope*);


#endif /* TRACK_H */
