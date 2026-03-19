#ifndef SLOPE_H
#define SLOPE_H
#define B1_FASTEST  3
#define B1_AVERAGE 11
#define R1_FASTEST  3
#define R1_AVERAGE 10
#define B2_FASTEST  5
#define B2_AVERAGE 21
#define R2_FASTEST  4
#define R2_AVERAGE 19
#define S1_FASTEST  9
#define S1_AVERAGE 34
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


#endif /* SLOPE_H */
