#include <sys/time.h>
#include <time.h>
#include "virtual_time.h"

vmsec_t 	last_virtual_time;
vmsec_t		frame_interval = 100;
vmsec_t		last_frame_time = 0;
vmsec_t		delta_time = 1;
struct timeval 	last_real_time;
double time_scale = 1.0;

vmsec_t get_time(void){
	return last_virtual_time;
}
vmsec_t get_dtime(void){
	return delta_time;
}
float 	get_dtime_sec(void){
	return delta_time / 1000.0;
}
/**
 * Returns the time of the outside world in msec
 */
static vmsec_t get_real_time(void){
	struct timeval now;
	gettimeofday(&now,NULL);
	return now.tv_sec*1000+now.tv_usec/1000;
}
void init_time(){
	gettimeofday(&last_real_time,NULL);
	last_virtual_time = last_real_time.tv_sec*1000 
				+ last_real_time.tv_usec/ 1000;
}
void set_time_scale(double scale){
	time_scale = scale;
}
double get_time_scale(void){
	return time_scale;
}
void next_time(void){
	struct timeval now;
	gettimeofday(&now,NULL);
	delta_time = (now.tv_sec*1000 + now.tv_usec/1000) -
				(last_real_time.tv_sec*1000 + last_real_time.tv_usec/1000);
	delta_time *= time_scale;
	last_real_time = now;
	last_virtual_time += delta_time;
}
void set_fps(int fps){
	frame_interval = 1000/fps;
}
void wait_frame(void){
	struct timespec w_time;
	vmsec_t now = get_real_time();
	while(last_frame_time + frame_interval > now){
		vmsec_t wait_time = last_frame_time + frame_interval - now;
		w_time.tv_sec = wait_time / 1000;
		w_time.tv_nsec = (wait_time % 1000) * 1000000;
		nanosleep(&w_time,NULL);
		now = get_real_time();
	}
	last_frame_time = now;
}







