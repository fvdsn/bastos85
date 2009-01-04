#include <stdlib.h> 
#include <stdio.h> 
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

/**called every frame on the player ship*/
static void action(particle_t*p){
#define MISSILE 0
	particle_t * missile = NULL;
	vec_t spread = vec_new(random()%100 - 50,random()%100 -50);
	particle_simple_action(p);
	if(!vec_zero(p->v)){
		p->vector[MISSILE] = p->v;
	}
	if(key_pressed(' ') && get_time() > p->timer[MISSILE]){
		p->timer[MISSILE] = get_time() + 1;
		missile = factory_create_v(p->box.pos,P_MISSILE);
		missile->v = vec_add(p->vector[MISSILE],p->v);
		missile->v = vec_add(missile->v,spread);
		missile->die_time = get_time() + random()%1000+1000;
	}
	if(key_pressed('r')){
		p->box = box_rotate(p->box,1);
	}
}
/**called when a missile dies, creates a new one in a random direction */
static void missile_die(particle_t*p){
	particle_t *missile = NULL;
	vec_t newdir = vec_new(random()%256 - 128, random()%256 - 128);
	missile = factory_create_v(p->box.pos,P_MISSILE);
	missile->v = vec_add(newdir,vec_scale(p->v,0.7));;
	missile->die_time = get_time() + random()%1000 + 250;
	missile->box = box_rotate(missile->box,random()%360);
	if(random()%100 <2){
		missile->die = NULL;
	}
}
/**makes a wall rotate on itself*/
static void wall_rotate(particle_t *p){
	p->box = box_rotate(p->box,0.5);
}
/**called when a missile collides something*/
static void missile_collide(particle_t *p,particle_t*s){
	if(!s->move){
		particle_set_color(p,1,0.1,0,0.1);
		particle_set_alt_color(p,1,0,0,0.1);
	}
}
int main(int argc, char**argv){
	world_t*w = world_new(50000);
	particle_t *p;
	printf("CONTROLS: WSAD : move the ship\n\tR: rotate the ship\n\tSPACEBAR: fire missiles\n\tN,M,P: control the time flow \n");
	world_set(w);
	background_set_color(0.05,0.05,0.05,1);
	
	/*------------------------------------------*\
	 * Player ship
	\*------------------------------------------*/
	p = particle_new(box_new(vec_new(0,0),24,32),10);
	p->draw = particle_draw_square;	
	p->move = particle_simple_move;
	p->action = action;
	p->vector[MISSILE] = vec_new(100,0);
	particle_set_color(p,1,0.3,0,0.5);
	particle_set_alt_color(p,1,0.5,0,0.5);
	particle_set_collides(p,1);
	particle_set_camera(p,1);
	particle_set_solid(p,0);
	factory_register(p,P_SHIP);
	
	/*------------------------------------------*\
	 * SOLID WALLS
	\*------------------------------------------*/
	p = particle_new(box_new(vec_new(0,0),50,50),1);
	particle_set_color(p,0.5,0.5,0.5,1);
	particle_set_alt_color(p,0.8,0.8,0.8,1);
	p->draw = particle_draw_square;
	particle_set_solid(p,1);
	factory_register(p,P_WALL);
	
	/*------------------------------------------*\
	 * BACKGROUND BLOCKS
	\*------------------------------------------*/
	p = particle_new(box_new(vec_new(0,0),50,50),-1);

	particle_set_color(p,0.1,0.1,0.1,1);
	particle_set_alt_color(p,0,0,0,1);
	p->draw = particle_draw_square;
	factory_register(p,P_BG);
	
	/*------------------------------------------*\
	 * MISSILES
	\*------------------------------------------*/
	p = particle_new(box_new(vec_new(0,0),20,20),2);
	particle_set_color(p,0,0.5,1,0.1);
	particle_set_alt_color(p,0.1,0.6,1,0.1);
	p->draw = particle_draw_square;
	p->move = particle_simple_move;
	p->collide = missile_collide;
	particle_set_collides(p,1);
	p->die = missile_die;
	factory_register(p,P_MISSILE);

	/*------------------------------------------*\
	 * WORLD INSTANCIATION
	\*------------------------------------------*/
	factory_create(0,0,P_SHIP);
	
	factory_create(100,100,P_WALL);
	factory_create(150,150,P_WALL);
	p=factory_create(50,0,P_WALL);
	p->box = box_rotate(p->box,45);

	factory_create(-100,-100,P_BG);
	factory_create(-150,-150,P_BG);
	factory_resized(-300,-150,300,-100,P_BG);
	
	factory_resized(-400,-200,400,-150,P_WALL);
	factory_resized(-400,200,400,150,P_WALL);
	factory_resized(400,50,350,-150,P_WALL);
	p = factory_resized(-400,150,-350,-50,P_WALL);
	p->think = wall_rotate;
	p->think_interval = 10;
	
	/*running the world*/

	return world_main_loop(argc,argv,w);
}
