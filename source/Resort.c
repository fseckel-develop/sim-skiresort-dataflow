#include "Resort.h"
#include <stdlib.h>
#include <stdio.h>


/*  create_resort():
 *  Allocates memory for a new resort and associates it with the given
 *  clock. Creates all resort parts and associates them if necessary.
 *  The fastest and average skiing times for all slopes are set.
 *      params: reference to clock
 *      return: reference to resort (or NULL)
 */
Resort* create_resort(Clock* clock) {
    if (clock) {
        Resort* resort = calloc(1, sizeof(Resort));
        if (!resort) {
            fprintf(stderr, "Error in create_resort: failed to allocate memory for resort\n");
            return NULL;
        }
        resort->clock = clock;

        resort->lift = create_lift(clock);

        resort->valley_station = create_station(VALLEY_STATION, resort->lift);
        resort->middle_station = create_station(MIDDLE_STATION, resort->lift);
        resort->summit_station = create_station(SUMMIT_STATION, resort->lift);

        resort->lift_queue_valley = create_lift_queue(VALLEY_LIFT_QUEUE, resort->valley_station);
        resort->lift_queue_middle_up = create_lift_queue(MIDDLE_LIFT_QUEUE_UP, resort->middle_station);
        resort->lift_queue_middle_down = create_lift_queue(MIDDLE_LIFT_QUEUE_DOWN, resort->middle_station);
        resort->lift_queue_summit = create_lift_queue(SUMMIT_LIFT_QUEUE, resort->summit_station);

        resort->slope_b1 = create_slope(SLOPE_B1, resort->middle_station, resort->valley_station, B1_FASTEST, B1_AVERAGE);
        resort->slope_r1 = create_slope(SLOPE_R1, resort->middle_station, resort->valley_station, R1_FASTEST, R1_AVERAGE);
        resort->slope_b2 = create_slope(SLOPE_B2, resort->summit_station, resort->middle_station, B2_FASTEST, B2_AVERAGE);
        resort->slope_r2 = create_slope(SLOPE_R2, resort->summit_station, resort->middle_station, R2_FASTEST, R2_AVERAGE);
        resort->slope_s1 = create_slope(SLOPE_S1, resort->summit_station, resort->valley_station, S1_FASTEST, S1_AVERAGE);

        resort->bus_stop = create_bus_stop(resort->valley_station, clock);
        resort->car_park = create_car_park(resort->valley_station, clock);
        resort->hotel = create_hotel(resort->valley_station, clock);

        resort->bistro = create_bistro(resort->middle_station);

        if (!resort_is_valid(resort)) {
            fprintf(stderr, "Error in create_resort: one or more invalid resources for resort\n");
            destroy_resort(resort);
            return NULL;
        }
        return resort;
    }
    return NULL;
}


/*  resort_is_valid():
 *  Checks if all resources of the given resort have been properly allocated
 *  and set up in cooperation with their error-prone create-functions.
 *      params: reference to resort
 *      return: TRUE or FALSE
 */
Boolean resort_is_valid(const Resort* resort) {
    if (resort) {
        return resort->lift && resort->valley_station && resort->middle_station && resort->summit_station
        && resort->lift_queue_valley && resort->lift_queue_middle_up && resort->lift_queue_middle_down
        && resort->lift_queue_summit && resort->slope_b1 && resort->slope_r1 && resort->slope_b2
        && resort->slope_r2 && resort->slope_s1 && resort->bus_stop && resort->car_park
        && resort->hotel && resort->bistro;
    }
    return FALSE;
}


/*  progressing_arrivals():
 *  Calls functions for managing arrivals at the given resort.
 *      params: reference to resort
 *      return: none
 */
void progressing_arrivals(const Resort* resort) {
    if (resort) {
        bus_is_arriving(resort->bus_stop);
        car_is_arriving(resort->car_park);
        guests_are_resting(resort->hotel);
        guests_walk_to_ski(resort->hotel);
    }
}


/*  progressing_lift_exits():
 *  Calls functions for managing lift exits at the given resort.
 *      params: reference to resort
 *      return: none
 */
void progressing_lift_exits(const Resort* resort) {
    if (resort) {
        /* Updating gondolas at every station */
        set_gondolas(resort->valley_station, resort->lift);
        set_gondolas(resort->middle_station, resort->lift);
        set_gondolas(resort->summit_station, resort->lift);
        /* Letting people exit gondolas at every station */
        exit_lift(resort->valley_station);
        exit_lift(resort->middle_station);
        exit_lift(resort->summit_station);
    }
}


/*  progressing_ski_slopes():
 *  Calls functions for managing ski slopes at the given resort.
 *      params: reference to resort
 *      return: none
 */
void progressing_ski_slopes(const Resort* resort) {
    if (resort) {
        /* Progressing skiers at every slope */
        people_are_skiing(resort->slope_b1);
        people_are_skiing(resort->slope_r1);
        people_are_skiing(resort->slope_b2);
        people_are_skiing(resort->slope_r2);
        people_are_skiing(resort->slope_s1);
        /* Letting people start skiing at every slope */
        let_people_enter(resort->slope_b1);
        let_people_enter(resort->slope_r1);
        let_people_enter(resort->slope_b2);
        let_people_enter(resort->slope_r2);
        let_people_enter(resort->slope_s1);
    }
}


/*  progressing_bistro():
 *  Calls functions for managing the bistro at the given resort.
 *      params: reference to resort
 *      return: none
 */
void progressing_bistro(const Resort* resort) {
    if (resort) {
        customers_are_eating(resort->bistro);
        welcome_customers(resort->bistro);
    }
}


/*  progressing_lift_queues():
 *  Calls functions for managing lift queues and lift entries at the given resort.
 *      params: reference to resort
 *      return: none
 */
void progressing_lift_queues(const Resort* resort) {
    if (resort) {
        /* Letting people fall in line at every lift queue */
        enqueue_people(resort->lift_queue_valley);
        enqueue_people(resort->lift_queue_middle_up);
        enqueue_people(resort->lift_queue_middle_down);
        enqueue_people(resort->lift_queue_summit);
        /* Updating the entry gondola for every lift queue */
        set_entry_gondola(resort->lift_queue_valley);
        set_entry_gondola(resort->lift_queue_middle_up);
        set_entry_gondola(resort->lift_queue_middle_down);
        set_entry_gondola(resort->lift_queue_summit);
        /* Letting people enter the lift at every lift queue */
        enter_lift(resort->lift_queue_valley);
        enter_lift(resort->lift_queue_middle_up);
        enter_lift(resort->lift_queue_middle_down);
        enter_lift(resort->lift_queue_summit);
    }
}


/*  progressing_departures():
 *  Calls functions for managing departures at the given resort.
 *      params: reference to resort
 *      return: none
 */
void progressing_departures(const Resort* resort) {
    if (resort) {
        /* Managing departures via the bus stop */
        return_to_bus_stop(resort->bus_stop);
        bus_is_departing(resort->bus_stop);
        /* Managing departures via the car park */
        return_to_car_park(resort->car_park);
        cars_are_departing(resort->car_park);
        /* Managing departures to the hotel */
        return_to_hotel(resort->hotel);
    }
}


/*  clean_up_resort():
 *  Calls functions for removing all left skiers from the given resort.
 *      params: reference to resort
 *      return: none
 */
void clean_up_resort(const Resort* resort) {
    if (resort) {
        /* Moving skiers from all slopes to the valley station */
        collect_skiers_with_snowmobile(resort->slope_b1, resort->valley_station);
        collect_skiers_with_snowmobile(resort->slope_r1, resort->valley_station);
        collect_skiers_with_snowmobile(resort->slope_b2, resort->valley_station);
        collect_skiers_with_snowmobile(resort->slope_r2, resort->valley_station);
        collect_skiers_with_snowmobile(resort->slope_s1, resort->valley_station);
        /* Initiating departure of bus-arriving people by taxi after last bus left */
        get_taxi_for_late_bus_passengers(resort->bus_stop);
    }
}


/*  current_visitor_count():
 *  Determines the current visitor count at the given resort.
 *      params: reference to resort
 *      return: none
 */
int current_visitor_count(const Resort* resort) {
    int visitor_count = 0;
    if (resort) {
        /* Increasing visitor count by person count of all stations */
        visitor_count += resort->valley_station->people_at_station->size;
        visitor_count += resort->middle_station->people_at_station->size;
        visitor_count += resort->summit_station->people_at_station->size;
        /* Increasing visitor count by passenger count of all lift queues */
        visitor_count += resort->lift_queue_valley->queue->size;
        visitor_count += resort->lift_queue_middle_up->queue->size;
        visitor_count += resort->lift_queue_middle_down->queue->size;
        visitor_count += resort->lift_queue_summit->queue->size;
        /* Increasing visitor count by skier count of all slopes */
        visitor_count += resort->slope_b1->skiers->size;
        visitor_count += resort->slope_r1->skiers->size;
        visitor_count += resort->slope_b2->skiers->size;
        visitor_count += resort->slope_r2->skiers->size;
        visitor_count += resort->slope_s1->skiers->size;
        /* Increasing visitor count by passenger count of bus stop */
        visitor_count += resort->bus_stop->queue->size;
        /* Increasing visitor count by customer count of bistro */
        visitor_count += resort->bistro->customers->size;
        /* Increasing visitor count by passenger count of lift */
        visitor_count += passengers_on_lift_between(resort->lift->valley_entry, resort->lift->valley_entry);
    }
    return visitor_count;
}


/*  total_ten_tickets():
 *  Determines the total count of ten-tickets at the given resort.
 *      params: reference to resort
 *      return: none
 */
int total_ten_tickets(const Resort* resort) {
    int ticket_count = 0;
    if (resort) {
        ticket_count += resort->lift_queue_valley->sold_ten_tickets;
        ticket_count += resort->lift_queue_middle_up->sold_ten_tickets;
        ticket_count += resort->lift_queue_middle_down->sold_ten_tickets;
        ticket_count += resort->lift_queue_summit->sold_ten_tickets;
    }
    return ticket_count;
}


/*  total_day_tickets():
 *  Determines the total count of day-tickets at the given resort.
 *      params: reference to resort
 *      return: none
 */
int total_day_tickets(const Resort* resort) {
    int ticket_count = 0;
    if (resort) {
        ticket_count += resort->lift_queue_valley->sold_day_tickets;
        ticket_count += resort->lift_queue_middle_up->sold_day_tickets;
        ticket_count += resort->lift_queue_middle_down->sold_day_tickets;
        ticket_count += resort->lift_queue_summit->sold_day_tickets;
    }
    return ticket_count;
}


/*  total_slope_entries():
 *  Determines the total count of slope entries at the given resort.
 *      params: reference to resort
 *      return: none
 */
int total_slope_entries(const Resort* resort) {
    int entry_count = 0;
    if (resort) {
        entry_count += resort->slope_b1->total_entries;
        entry_count += resort->slope_r1->total_entries;
        entry_count += resort->slope_b2->total_entries;
        entry_count += resort->slope_r2->total_entries;
        entry_count += resort->slope_s1->total_entries;
    }
    return entry_count;
}


/*  print_resort_state():
 *  Clears the console and prints data of the given resort within graphical elements.
 *      params: reference to resort
 *      return: none
 */
void print_resort_state(const Resort* resort) {
    if (!resort) {
        return;
    }
    system("clear");
    printf("  Skiers at Resort: %4d              ,====================,                                          O=OO=OO=O\n",
        current_visitor_count(resort));
    printf("  Sold Ten-Tickets: %4d      ,-------|   SUMMIT STATION   |-------,-----------------,-----------,--[}0   ,---0{]\n",
        total_ten_tickets(resort));
    printf("  Sold Day-Tickets: %4d      |       '===================='       | Queue Down: %3d |           '--[}O---'   O{]\n",
        total_day_tickets(resort), resort->lift_queue_summit->queue->size);
    printf("  Slope Entries:   %5d      |               |    |               '-----------------'              [}O       O{]\n",
        total_slope_entries(resort));
    printf("                              |     ,---------|    |---------,                             ,-------------, ,-------------,\n");
    printf("                              |     | R2: %3d |    | B2: %3d |                             | (M->S): %3d | | (S->M): %3d |\n",
        resort->slope_r2->skiers->size, resort->slope_b2->skiers->size,
        passengers_on_lift_between(resort->lift->middle_upward, resort->lift->summit_exit),
        passengers_on_lift_between(resort->lift->summit_entry, resort->lift->middle_downward));
    printf("            ,-----------------'     '---------|    |---------'      ,---------------,      '-------------' '-------------'\n");
    printf("            |                                 |    |                | Queue Up: %3d |               [}O       O{]\n",
        resort->lift_queue_middle_up->queue->size);
    printf("            |     ,-------------,     ,====================,--------'---------------'------------,  [}O       O{]\n");
    printf("            |     | Bistro: %3d |-----|   MIDDLE STATION   |                                     '--[}0   ,---0{]\n",
        resort->bistro->customers->size);
    printf("            |     '-------------'     '===================='-------,-----------------,--------------[}O---'   O{]\n");
    printf("            |                                 |    |               | Queue Down: %3d |              [}O       O{]\n",
        resort->lift_queue_middle_down->queue->size);
    printf("            '-------,---------,     ,---------|    |---------,     '-----------------'     ,-------------, ,-------------,\n");
    printf("                    | S1: %3d |     | R1: %3d |    | B1: %3d |                             | (V->M): %3d | | (M->V): %3d |\n",
        resort->slope_s1->skiers->size, resort->slope_r1->skiers->size, resort->slope_b1->skiers->size,
        passengers_on_lift_between(resort->lift->valley_entry, resort->lift->middle_upward),
        passengers_on_lift_between(resort->lift->middle_downward, resort->lift->valley_exit));
    printf("                    '---------|     '---------|    |---------'                             '-------------' '-------------'\n");
    printf("                              |               |    |                ,---------------,               [}O       O{]\n");
    printf("   ,=========,                |       ,====================,        | Queue Up: %3d |            ,--[}O---,   O{]\n",
        resort->lift_queue_valley->queue->size);
    printf("   |  "); print_time(resort->clock->time);
    printf("  |                '-------|   VALLEY STATION   |--------'---------------'------------'--[}0   '---0{]\n");
    printf("   '========='                        |====================|                                          O=OO=OO=O\n");
    printf("                                      |         |          |\n");
    /* Is the clock currently running? */
    if (resort->clock->speed != STOPPED) {
        /* Simulation can be paused */
        printf("   ...(P)ause       ,-----------------|   ,-----'-----,    |---------------,\n");
    }
    else {
        /* Clock is stopped, and simulation can be resumed */
        printf("   ...(R)esume      ,-----------------|   ,-----'-----,    |---------------,\n");
    }
    /* Which speed does the clock currently have? */
    switch (resort->clock->speed) {
        case NORMAL: {
            /* Speed of clock can be put to Turbo */
            printf("   ...(T)urbo       | Parked Cars: %2d |   | Hotel: %2d |    | Bus Stop: %3d |",
            resort->car_park->car_count, resort->hotel->present_guests->size, resort->bus_stop->queue->size);
            break;
        }
        case TURBO: {
            /* Speed of clock can be put to Normal */
            printf("   ...(S)low        | Parked Cars: %2d |   | Hotel: %2d |    | Bus Stop: %3d |",
            resort->car_park->car_count, resort->hotel->present_guests->size, resort->bus_stop->queue->size);
            break;
        }
        case STOPPED: {
            /* Speed of clock cannot be changed */
            printf("                    | Parked Cars: %2d |   | Hotel: %2d |    | Bus Stop: %3d |",
            resort->car_park->car_count, resort->hotel->present_guests->size, resort->bus_stop->queue->size);
            break;
        }
        default: {}
    }
    /* Is bus present at the bus stop? */
    if (resort->bus_stop->bus != NULL) {
        printf("===(H)  Bus is present\n");
    }
    else {
        printf("===(H)\n");
    }
    printf("   ...(A)bort       '-----------------'   '-----------'    '---------------'\n");
}


/*  destroy_resort():
 *  Nullifies the associated clock, destroys all resort
 *  parts and frees the memory of the given resort.
 *      params: reference to resort
 *      return: none
 */
void destroy_resort(Resort* resort) {
    if (resort) {
        resort->clock = NULL;
        destroy_lift(resort->lift);
        destroy_station(resort->valley_station);
        destroy_station(resort->middle_station);
        destroy_station(resort->summit_station);
        destroy_lift_queue(resort->lift_queue_valley);
        destroy_lift_queue(resort->lift_queue_middle_up);
        destroy_lift_queue(resort->lift_queue_middle_down);
        destroy_lift_queue(resort->lift_queue_summit);
        destroy_slope(resort->slope_b1);
        destroy_slope(resort->slope_b2);
        destroy_slope(resort->slope_r1);
        destroy_slope(resort->slope_r2);
        destroy_slope(resort->slope_s1);
        destroy_bus_stop(resort->bus_stop);
        destroy_car_park(resort->car_park);
        destroy_hotel(resort->hotel);
        destroy_bistro(resort->bistro);
        free(resort);
    }
}
