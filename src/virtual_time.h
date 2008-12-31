#ifndef __2D_VIRTUAL_TIME_H__
#define __2D_VIRTUAL_TIME_H__
/** 
 * A type used to store miliseconds
 */
typedef unsigned long vmsec_t;	
/**
 * Returns the current time in msec
 * @return the current time in msec
 */
vmsec_t get_time(void);
/**
 * Returns the time elapsed since the last frame
 * @return the time between this frame and the last one in msecs
 */
vmsec_t get_dtime(void);
/**
 * Returns the time elapsed sine the last frame in seconds
 * @return the time between this frame and the last one in msecs
 */
float get_dtime_sec(void);
/**
 * Sets the time to the next frame. Between two next_time(), 
 * all get_time() return exactly the same value
 */
void next_time(void);
/**
 * Sets the rate at wich the time advances. ex : 0.5 : time is two time
 * slower, 2.0 : time is two time faster, 0 : the time stops
 * @param scale : the time scale
 */
void set_time_scale(double scale);
/**
 * Returns the time scale :
 * @return a double representing the current time scaling factor
 */
double get_time_scale(void);
/**
 * Initializes the Virtual Time System, must be called before any other time
 * call
 */
void init_time(void);
/**
 * Sets the target frame per second. The game won't run faster than that.
 * @param fps  : the number of frame per second
 */
void set_fps(int fps);
/**
 * Wait the time remaining before starting to draw the next frame. 
 * see set_fps();
 */
void wait_frame(void);

#endif

