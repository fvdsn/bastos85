#ifndef __2D_WORLD_H__
#define __2D_WORLD_H__
#include "particle.h"

/**
 * A structure representing the world to be rendered.
 */
typedef struct world_s{
	/** The maximum amount of particles in the world */
	int max_particles;
	particle_t **all_particle;
	int thinking_particle_count;
	particle_t **thinking_particle;
	int moving_particle_count;
	particle_t **moving_particle;
	int visible_particle_count;
	particle_t **visible_particle;
}world_t;
/**
 * Creates a new empty world
 * @param max_particles : the maximum count of dead + alive particle that
 * there can be on one frame
 * @return a new empty world
 */
world_t *world_new(int max_particles);
/**
 * Returns the current active world
 * @return : the current active world
 */
world_t *world_get(void);
/**
 * Sets the current active world
 * @param : the current active world
 */
void	 world_set(world_t *w);
/**
 * Iterates over all the world particles.
 * @param w : the world where we want to get our particles
 * @param id : the iteration index . the iterator will check for particle
 * at index greater or equal to this value, so it should be 0 if you want
 * to get all the particles. After an iteration it will be set to the next
 * possible id so that you can continue the iteration.
 * @return a particle if there is one, or NULL if we have iterated over all
 * the particles
 */
particle_t *world_next_particle(world_t*w, int *id);
/**
 * Iterates over all drawable particles (see world_next_particle for usage)
 */
particle_t *world_next_drawable(world_t*w, int *id);
/**
 * Iterates over all moving particles (see world_next_particle for usage)
 */
particle_t *world_next_moving(world_t*w, int *id);
/**
 * Iterates over all thinking particles (see world_next_particle for usage)
 */
particle_t *world_next_thinking(world_t*w, int *id);
/**
 * Iterates over all collidable particles within range 
 * (see world_next_particle for usage)
 * @param range : a box representing the zone where we want to check for
 * collisions
 */
particle_t *world_next_solid(world_t*w,box_t range, int *id);
/**
 * Adds a particle to the world. It will be fully effective only on the next
 * frame. Adding a particle multiple time is a very bad idea. 
 * @param w : the world where we add our particle
 * @param p : the particle we are adding
 */
void world_add_particle(world_t *w, particle_t *p);
/**
 * Makes sure that all particle sets are up to date so that the iterators can
 * be used correctly
 */
void world_setup_iterators(world_t*w);
/**
 * Main loop of the engine.
 * It loops infinitely performing for each frame :
 *  ->setting up the iterators
 *  -> catching keyboards events
 *  -> increasing virtual time
 *  -> performing the physics
 *  -> allowing particles to think
 *  -> draws the particles
 *  -> emit sound
 *  -> collect dead particles
 * In that order.
 */
void do_world(void);
/**
 * Perform physics : allow particles to move by calling their move functions.
 * Detect collisions, call the collide function of the particle then prevent
 * possible intersections. Computes damage and call the damage function of
 * the particles
 */
void do_physics(world_t*w);
/**
 * Allow the particle to think, performs all the timers, then kill the 
 * particles if necessary
 */
void do_think(world_t *w);
/**
 * Draws the background and the particles
 */
void do_graphics(world_t*w);
/**
 * Plays the music and the sounds 
 */
void do_sounds(world_t*w);
/**
 * Free dead particles and other stuff
 */
void do_garbage_collect(world_t*w);
/**
 * Displays the program main window
 */
void main_window(int argc, char**argv, int sizex, int sizey);
/**
 * Displays the world in a window. This is the function you want
 * to call in your main program after you have set up all the 
 * worlds and particles, its return value is what your main 
 * program should return. 
 * @param argc : the number of arguments provided by the command line at the
 * program launch.
 * @param argv : an array of strings containing each argument value
 * @param w : the world that will initially be displayed, cannot be NULL
 */
int world_main_loop(int argc, char**argv, world_t *w);

#endif

