#ifndef __2D_DRAW_H__
#define __2D_DRAW_H__
#include "particle.h"
#include "vector.h"
/**
 * Initializes the drawing stuff.
 */
void draw_init(void);
/** 
 * Called when the window changes sizes, recomputes
 * the perspective
 */
void draw_reshape(int w, int h);
/**
 * Draws the background
 */
void draw_background(void);
/**
 * A simple draw method for particles : It draws
 * them as a plain square with the same color, 
 * position and size as the particle
 */
void particle_draw_square(particle_t *self);
/**
 * Pops all the matrixes after the frame is
 * drawn
 */
void draw_flush(void);
/**
 * Centers the camera on the position
 * @param pos a vector corresponding to the camera position (in world pixel units)
 * it is (0,0) by default
 */
void camera_set_position(vec_t pos);
/**
 * Returns the center position of the camera
 * @return a vector pointing to the center of the camera in world pixel units
 */
vec_t camera_get_position(void);
/**
 * Sets the camera position but doesn't move it if the precedent position
 * was close enough, allowing a free zone where the followed particle
 * can move without making the camera move.
 * @param pos : the new candidate position
 * @param freezone in [0,1] percentage of the windows that acts as a freezone.
 * @param elasticity in [0,1] how fast the camera reacts to the freezone 
 * (0 no * reaction and 1 instant reaction )
 */
void camera_set_zone(vec_t pos, float freezone, float elasticity);
/**
 * Sets the background color.
 * The alpha component is only used for composite windows.
 * @param r : in [0,1] Red
 * @param g : in [0,1] Green
 * @param b : in [0,1] Blue
 * @param alpha : in [0,1] the transparency
 */
void background_set_color(float r,float g, float b, float alpha);

#endif

