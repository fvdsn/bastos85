#ifndef __2D_PARTICLE_H__
#define __2D_PARTICLE_H__
#include "virtual_time.h"
#include "vector.h"

#define PARTICLE_PARAM_COUNT 8
typedef struct particle_s{
	box_t box;		/*position and size*/
	float z;		/*depth*/
	vec_t v;		/*speed*/
	vec_t a;		/*acceleration*/
	float mass;		
	vec_t air_friction;	
	vec_t ground_friction;
	int solid;		/*1 if the particles collides*/
	int life;		/*dead if <= 0 */
	vmsec_t die_time;	/*time when the particle will die*/
	vmsec_t next_think;	/*time when the particle will think*/
	vmsec_t think_interval; /*time between think*/
	int think_count;	/*number of time the particle has thought*/
	int flags;	
	int camera;	/*1 if the camera follows this particle*/
	float color[4];		
	float param[PARTICLE_PARAM_COUNT];
	vmsec_t timer[PARTICLE_PARAM_COUNT];
	vmsec_t timer_interval[PARTICLE_PARAM_COUNT];
	vec_t vector[PARTICLE_PARAM_COUNT];
	struct particle_s *parent;
	void (*move)(struct particle_s *self);
	void (*damage)(struct particle_s *self,  struct particle_s *source, int damage);
	void (*collide)(struct particle_s *self, struct particle_s *source);
	void (*die)(struct particle_s *self);
	void (*think)(struct particle_s *self);
	void (*draw)(struct particle_s *self);
	void (*action)(struct particle_s *self);
}particle_t;
/**
 * Creates a new particle
 * The default think interval is 100msec
 * the default life is 1
 * All other values are either NULL or 0
 * @param box : the size and position of the particle
 * @param z : the depth of the particle, positive z are in front.
 */
particle_t *particle_new(box_t box, float z);
/**
 * Frees a particle
 */
void particle_free(particle_t*p);
/**
 * Return true if the particle is dead and can be freed
 */
int  particle_is_dead(const particle_t*p);
/**
 * Makes the particle dead
 */
void particle_kill(particle_t*p);
/**
 * Makes the particle collide with other particles
 * @param p : the modified particle
 * @param trueorfalse : 1 if the particle collides, 0 if it doesn't
 */
void particle_set_solid(particle_t*p, int trueorfalse);
/**
 * Return true if the particle collides
 * @param p : the particle
 * @return : 1 if the particle collides, 0 if it doesn't
 */
int particle_is_solid(particle_t*p);
/**
 * Makes the camera follow this particle
 * @param p : the followed particle
 * @param trueorfalse : if 1 the camera will follow the particle, if 0 it won't
 */
void particle_set_camera(particle_t *p, int trueorfalse);
/**
 * Checks if the particle is followed by the camera
 * @param p : the particle
 * @return : 1 if the particle is followed by the camera, 0 if it isn't.
 */
int particle_is_camera(particle_t *p);
/**
 * Sets a flags to true or false
 * @param p : the modified particle
 * @param id : in [0,31] the id of the flag
 * @param trueorfalse : in [0,1] the new value of the flag
 */
void particle_set_flag(particle_t*p, int id, int trueorfalse);
/**
 * Returns a flag from the particle
 * @param p : the particle we get the flag from
 * @param id: in [0,31] the id of the flag
 * @returns 1 or 0 depending on the previously set value. 
 */
int  particle_get_flag(const particle_t *p, int id);
/**
 * Sets the color of the particle
 * @param p: the modified particle
 * @param r : in [0,1] the red component
 * @param g : in [0,1] the green component
 * @param b : in [0,1] the blue component
 * @param a : in [0,1] the opacity component
 */
void particle_set_color(particle_t*p,float r, float g, float b, float a);
/**
 * Sets the lifetime of the particle
 * @param p : the modified particle
 * @param time : the particle will die 'time' miliseconds from now
 */
void particle_set_lifetime(particle_t*p, vmsec_t time);
/**
 * Sets a parent to the particle. Motion of the particle is based on
 * the parent coordinates
 * @param p : the modified particle :
 * @param parent : the parent of p
 */
void particle_set_parent(particle_t*p, const particle_t* parent);
/**
 * Stores a parameter in the particle
 * @param p : the modified particle
 * @param id : in [0, PARTICLE_PARAM_COUNT - 1 ], the id of the param
 * @param param : the value of the parameter
 */
void particle_set_param(particle_t*p, int id, float param);
/**
 * Gets a parameter from a particle
 * @param p : the particle where we get the param.
 * @param id: in [0, PARTICLE_PARAM_COUNT - 1 ] the id of the param
 * @return : the value stored in the parameter
 */
float particle_get_param(particle_t*p, int id);
/**
 * A function used to sort the particles by increasing z values
 */
int particle_z_sort(const void *a, const void *b);
/*simple draw functions */
void particle_draw_console(particle_t *self);
/*simple move functions */
void particle_simple_move(particle_t *self);
/*simple think functions*/
void particle_simple_think(particle_t *self);
/*simple particle action*/
void particle_simple_action(particle_t *self);


#endif
