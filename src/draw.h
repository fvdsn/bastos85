#ifndef __2D_DRAW_H__
#define __2D_DRAW_H__
#include "particle.h"

void draw_init(void);
void draw_reshape(int w, int h);
void draw_background(void);
void particle_draw_square(particle_t *self);
void draw_flush(void);

#endif

