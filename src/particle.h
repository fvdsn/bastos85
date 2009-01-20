#ifndef __2D_PARTICLE_H__
#define __2D_PARTICLE_H__

#include "virtual_time.h"
#include "vector.h"
#include "property.h"
#include "model.h"

#define PARTICLE_PARAM_COUNT 4

typedef struct ptimer_s ptimer_t;
typedef struct particle_s particle_t;

struct ptimer_s{
	vmsec_t next_time;
	vmsec_t timer_interval;
	void (*timer)(particle_t *self);
};

#define PHYS_SOLID 1
#define PHYS_COLLIDE 2

struct particle_s{
	/* PHYSICS */
	box_t 	box;		/*position and size*/
	float 	z;		/*depth*/
	vec_t 	v;		/*speed*/
	vec_t 	a;		/*acceleration*/
	float	mass;		
	vec_t 	air_friction;	
	vec_t 	ground_friction;
	int 	physflag;		/*solid, collides, etc...*/
	int	group;			/*in [0, 31]*/
	int	collide_group;		/*particle collide only 
					  with particles with matching collide
					  group*/
	/* LOGIC */
	int	team;
	int 	life;		/*dead if <= 0 */
	vmsec_t die_time;
	struct particle_s *parent;

	/* GRAPHICS */
	int 	camera;		/*1 if the camera follows this particle*/
	float 	color[4];		
	float 	altcolor[4];
	model_t *model[PARTICLE_PARAM_COUNT];
	
	/*PARAMS*/
	int 	 flags;	
	int	 ptimer_count;
	int	 has_prop;
	float 	 param[PARTICLE_PARAM_COUNT];
	vec_t 	 vector[PARTICLE_PARAM_COUNT];
	vmsec_t  time[PARTICLE_PARAM_COUNT];
	ptimer_t ptimer[PARTICLE_PARAM_COUNT];
	nprop_t  nprop[PARTICLE_PARAM_COUNT];
	vprop_t  vprop[PARTICLE_PARAM_COUNT];

	/*CALLBACKS*/
	void (*move)(struct particle_s *self);
	void (*damage)(struct particle_s *self,  struct particle_s *source, int damage);
	void (*collide)(struct particle_s *self, struct particle_s *source);
	void (*die)(struct particle_s *self);
	void (*draw)(struct particle_s *self);
	void (*action)(struct particle_s *self);
};

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
 * The particle will modify colliding particles trajectory.
 * @param p : the modified particle
 * @param trueorfalse : 1 if the particle collides, 0 if it doesn't
 */
void particle_set_solid(particle_t*p, int trueorfalse);
/**
 * Return true if the particle recieves collisions (like wall)
 * @param p : the particle
 * @return : 1 if the particle recieves collisions, 0 if it doesn't
 */
int particle_is_solid(particle_t*p);
/**
 * Makes the particle modify its trajectory when colliding with a solid object
 * @param p : the modified particle
 * @param trueorfalse : the particle will collide if trueorfalse is 1 it will
 * not if it is 0.
 */
void particle_set_collides(particle_t*p, int trueorfalse);
/**
 * Return true if the particles collides 
 * smoke collides on wall but is not solid
 * collides should only be set if the particle movement will be
 * influenced by the collision. 
 * @param p : the particle
 * @return : 1 if the particle collides 0 if it doesn't
 */
int particle_collides(particle_t*p);
int particle_collide_with(particle_t *a, particle_t *b);
void particle_set_group(particle_t*p,int group);
void particle_toggle_collide_all(particle_t*p);
void particle_toggle_collide_group(particle_t*p, int group);
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
 * Sets the alternative color of the particle, used for outline and
 * overlays
 * @param p: the modified particle
 * @param r : in [0,1] the red component
 * @param g : in [0,1] the green component
 * @param b : in [0,1] the blue component
 * @param a : in [0,1] the opacity component
 */
void particle_set_alt_color(particle_t*p,float r, float g, float b, float a);
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

void particle_add_timer(particle_t*p, void (*func)(particle_t*p),vmsec_t timer);
int  particle_has_timer(particle_t*p);
void particle_do_timer(particle_t*p,vmsec_t now);

int	particle_has_prop(particle_t*p);

void    particle_set_nprop(particle_t*p, int id, nprop_t np);
nprop_t particle_get_nprop(particle_t*p, int id);
void 	particle_do_nprop(particle_t*p,  float sec_dt);

void    particle_set_vprop(particle_t*p, int id, vprop_t vp);
nprop_t particle_get_vprop(particle_t*p, int id);
void 	particle_do_vprop(particle_t*p, float sec_dt);

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
