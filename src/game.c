#include <stdlib.h> 
#include "world.h"
#include "vector.h"
#include "particle.h"
#include "factory.h"
#include "keyboard.h"
#include "draw.h"
#include "virtual_time.h"

#define P_SHIP 1
#define P_WALL 2
#define P_BG   3
#define P_MISSILE 4

static void action(particle_t*p){
	particle_t * missile = NULL;
	particle_simple_action(p);
#define MISSILE 0
	if(!vec_zero(p->v)){
		p->vector[MISSILE] = p->v;
	}
	if(key_pressed(' ') && get_time() > p->timer[MISSILE]){
		p->timer[MISSILE] = get_time() + 50;
		missile = factory_create_v(p->box.pos,P_MISSILE);
		missile->v = vec_add(p->vector[MISSILE],p->v);
		missile->die_time = get_time() + 1000;
	}
}

int main(int argc, char**argv){
	world_t*w = world_new(1000);
	particle_t *p;
	world_set(w);
	/* Player ship*/
	p = particle_new(box_new(vec_new(0,0),vec_new(8,16),0),0);
	p->draw = particle_draw_square;	
	p->move = particle_simple_move;
	p->think = particle_simple_think;
	p->action = action;
	p->vector[MISSILE] = vec_new(100,0);
	particle_set_color(p,0.7,0.7,0,1);
	particle_set_solid(p,1);
	factory_register(p,P_SHIP);

	p = particle_new(box_new(vec_new(0,0),vec_new(50,50),0),1);
	particle_set_color(p,0.5,0.5,0.5,1);
	p->draw = particle_draw_square;
	particle_set_solid(p,1);
	factory_register(p,P_WALL);

	p = particle_new(box_new(vec_new(0,0),vec_new(50,50),0),-1);

	particle_set_color(p,0.1,0.1,0.1,1);
	p->draw = particle_draw_square;
	factory_register(p,P_BG);

	p = particle_new(box_new(vec_new(0,0),vec_new(4,4),0),2);
	particle_set_color(p,1,0,0,1);
	p->draw = particle_draw_square;
	p->move = particle_simple_move;
	factory_register(p,P_MISSILE);
	
	/*instanciate particles*/

	factory_create(0,0,P_SHIP);
	
	factory_create(100,100,P_WALL);
	factory_create(150,150,P_WALL);
	factory_create(50,0,P_WALL);

	factory_create(-100,-100,P_BG);
	factory_create(-150,-150,P_BG);
	factory_resized(-300,-150,300,-100,P_BG);
	
	/*running the world*/

	return world_main_loop(argc,argv,w);
}
