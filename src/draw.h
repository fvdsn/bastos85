#ifndef __2D_DRAW_H__
#define __2D_DRAW_H__
#include "particle.h"
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

#endif

