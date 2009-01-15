#ifndef __2D_GRID_H__
#define __2D_GRID_H__
#include "vector.h"
#include "particle.h"
#define MAX_PARTICLE_PER_NODE 100
#define MAX_PARTICLE_PER_ITERATOR 1000

typedef struct grid_node_s{
	int particle_count;
	particle_t *particle[MAX_PARTICLE_PER_NODE];
}grid_node_t;

typedef struct grid_iterator_s{
	int particle_count;
	particle_t *particle[MAX_PARTICLE_PER_ITERATOR];
}grid_iterator_t;

typedef struct grid_s{
	int nw;
	int nh;
	float nsize;
	grid_node_t *node;
	grid_iterator_t it;
}grid_t;

grid_t *grid_new(int nw, int nh, float nsize);
grid_iterator_t *grid_iterator_get(grid_t *g);	/*bad idea*/

void	grid_free(grid_t *g);
void	grid_reset(grid_t *g);
void	grid_add_particle(grid_t *g,particle_t *p);

grid_iterator_t *grid_get_particle(grid_t*,box_t b);
#endif
