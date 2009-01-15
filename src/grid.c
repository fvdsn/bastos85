#include <stdlib.h>
#include <string.h>
#include "grid.h"

grid_t *grid_new(int nw, int nh, float nsize){
	grid_t * g = (grid_t*)malloc(sizeof(grid_t));
	memset(g,0,sizeof(grid_t));
	g->node = (grid_node_t*)malloc(sizeof(grid_node_t)*nw*nh);
	memset(g->node,0,sizeof(grid_node_t)*nw*nh);
	g->nw = nw;
	g->nh = nh; 
	g->nsize = nsize;
	return g;
}
void grid_free(grid_t *g)
grid_iterator_t *grid_iterator_get(grid_t*g){
	return g->it;
}


	
