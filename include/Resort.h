#ifndef RESORT_H
#define RESORT_H
#include "LiftQueue.h"
#include "Slope.h"
#include "BusStop.h"
#include "CarPark.h"
#include "Hotel.h"
#include "Bistro.h"


/*  Attributes of Resort:
 *   - reference to the main clock
 *   - reference to the lift
 *   - references to stations (valley, middle, summit)
 *   - references to lift queues (valley, middle up, middle down, summit)
 *   - references to slopes (B1, R1, B2, R2, S1)
 *   - reference to the bus stop
 *   - reference to the car park
 *   - reference to the hotel
 *   - reference to the bistro
 */
typedef struct Resort {
    const Clock* clock;
    Lift* lift;
    Station* valley_station;
    Station* middle_station;
    Station* summit_station;
    LiftQueue* lift_queue_valley;
    LiftQueue* lift_queue_middle_up;
    LiftQueue* lift_queue_middle_down;
    LiftQueue* lift_queue_summit;
    Slope* slope_b1;
    Slope* slope_r1;
    Slope* slope_b2;
    Slope* slope_r2;
    Slope* slope_s1;
    BusStop* bus_stop;
    CarPark* car_park;
    Hotel* hotel;
    Bistro* bistro;
} Resort;


Resort* create_resort(Clock*);
Boolean resort_is_valid(const Resort*);
void progressing_arrivals(const Resort*);
void progressing_lift_exits(const Resort*);
void progressing_ski_slopes(const Resort*);
void progressing_bistro(const Resort*);
void progressing_lift_queues(const Resort*);
void progressing_departures(const Resort*);
void clean_up_resort(const Resort*);
int current_visitor_count(const Resort*);
int total_ten_tickets(const Resort*);
int total_day_tickets(const Resort*);
int total_slope_entries(const Resort*);
void print_resort_state(const Resort*);
void destroy_resort(Resort*);


#endif /* RESORT_H */
