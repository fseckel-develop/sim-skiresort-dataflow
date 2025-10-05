#ifndef CLOCK_H
#define CLOCK_H


/*  Attributes of Time:
 *   - hours
 *   - minutes
 *   - seconds
 */
typedef struct Time {
    int h;
    int min;
    int s;
} Time;


/* Possible clock speeds */
typedef enum Speed {
    STOPPED,
    NORMAL,
    TURBO
} Speed;


/*  Attributes of Clock:
 *   - current time
 *   - current speed
 *   - last speed for possible reset
 */
typedef struct Clock {
    Time time;
    Speed speed;
    Speed last_speed;
} Clock;


Clock* init_clock(Time);
void stop_clock(Clock*);
void set_normal_speed(Clock*);
void set_turbo_speed(Clock*);
void set_last_speed(Clock*);
void wait_seconds(double);
void tick_tack(Clock*);
void destroy_clock(Clock*);
Time t(int, int, int);
Time add(Time, Time);
int min(Time);
int s(Time);
void print_time(Time);


#endif /* CLOCK_H */
