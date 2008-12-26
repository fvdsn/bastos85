#ifndef __2D_FACTORY_H__
#define __2D_FACTORY_H__
#include "vector.h"
#include "particle.h"


#define MAX_KLASS_COUNT 128

void factory_register(particle_t *p, int klass);
particle_t *factory_create(float x, float y, int klass);
particle_t *factory_create_v(vec_t pos, int klass);
particle_t *factory_resized(float blx, float bly, float rtx, float rty, int klass);
particle_t *factory_resized_v(vec_t bl, vec_t tr, int klass);

#endif
