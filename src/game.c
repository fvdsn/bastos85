#include <stdlib.h> 
#include <stdio.h> 
#include "world.h"
#include "vector.h"
#include "particle.h"
#include "factory.h"
#include "keyboard.h"
#include "draw.h"
#include "virtual_time.h"
#include "game.h"



/**makes a wall rotate on itself*/
static void wall_rotate(particle_t *p){
	p->box = box_rotate(p->box,0.5);
}

int main(int argc, char**argv){
	world_t*w = world_new(50000);
	particle_t *p;
	printf("CONTROLS: WSAD : move the ship\n\tR: rotate the ship\n\tSPACEBAR: fire missiles\n\tN,M,P: control the time flow \n");
	world_set(w);
	background_set_color(0.03,0.04,0.015,1);

	/*------------------------------------------*\
	 * PLAYER SHIP
	\*------------------------------------------*/
	
	game_register_ship();

	/*------------------------------------------*\
	 * SOLID WALLS
	\*------------------------------------------*/
	p = particle_new(box_new(vec_new(0,0),50,50),1);
	particle_set_color(p,0.24,0.2,0.14,1);
	particle_set_alt_color(p,0.5,0.2,0.1,1);
	p->draw = particle_draw_square;
	particle_set_solid(p,1);
	factory_register(p,P_WALL);
	
	/*------------------------------------------*\
	 * BACKGROUND BLOCKS
	\*------------------------------------------*/
	p = particle_new(box_new(vec_new(0,0),50,50),-1);

	particle_set_color(p,0.1,0.02,0.01,1);
	particle_set_alt_color(p,0.2,0.02,0,1);
	p->draw = particle_draw_square;
	factory_register(p,P_BG);

	/*------------------------------------------*\
	 * WORLD INSTANCIATION
	\*------------------------------------------*/
	factory_create(0,0,P_SHIP);
	
	factory_create(100,100,P_WALL);
	factory_create(150,150,P_WALL);
	p=factory_create(50,0,P_WALL);
	p->box = box_rotate(p->box,45);

	factory_create(-100,400,P_WALL);
	factory_create(0,400,P_WALL);
	factory_create(100,400,P_WALL);
	factory_create(200,400,P_WALL);
	factory_create(300,400,P_WALL);
	factory_create(400,400,P_WALL);

	factory_create(-100,-100,P_BG);
	factory_create(-150,-150,P_BG);
	factory_resized(-300,-150,300,-100,P_BG);
	
	factory_resized(-400,-200,400,-150,P_WALL);
	factory_resized(-400,200,400,150,P_WALL);
	factory_resized(400,50,350,-150,P_WALL);
	p = factory_resized(-400,150,-350,-50,P_WALL);
	particle_add_timer(p,wall_rotate,10);
	
	factory_resized(-1000,-1000,1000,-1100,P_WALL);
	factory_resized(-1000,1000,1000,1100,P_WALL);
	factory_resized(-1000,-1000,-900,1000,P_WALL);
	factory_resized(900,-1000,1000,1000,P_WALL);
	/*running the world*/

	return world_main_loop(argc,argv,w);
}
