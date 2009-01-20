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

/*------------------------------------------*\
 * Player ship
\*------------------------------------------*/

static void ship_action(particle_t*p){
#define MISSILE 0
#define SHIP_SPEED 350
#define SHIP_ACCEL 2000
#define SHIP_VSPEED 0
#define SHIP_HSPEED 1
	particle_t * missile = NULL;
	particle_t * trail = NULL;
	vec_t spread = vec_new(random()%100 - 50,random()%100 -50);
	int boost = 0;
	
	nprop_t vspeed = particle_get_nprop(p,SHIP_VSPEED);
	nprop_t hspeed = particle_get_nprop(p,SHIP_HSPEED);
	if(!key_pressed('y')){
	if(key_pressed('a')){
		if(nprop_get(hspeed) > 100 ){
			hspeed = nprop_set_hard(hspeed,0);
			boost = 1;
		}
		hspeed = nprop_set(hspeed,-SHIP_SPEED);
	}else if(key_pressed('d')){
		if(nprop_get(hspeed) < -100 ){
			hspeed = nprop_set_hard(hspeed,0);
			boost = 1;
		}
		hspeed = nprop_set(hspeed,SHIP_SPEED);
	}else{
		hspeed = nprop_set(hspeed,0);
	}
	if(key_pressed('w')){
		if(nprop_get(vspeed) < -100 ){
			vspeed = nprop_set_hard(vspeed,0);
			boost = 1;
		}
		vspeed = nprop_set(vspeed,SHIP_SPEED);
	}else if(key_pressed('s')){
		if(nprop_get(vspeed) > 100 ){
			vspeed = nprop_set_hard(vspeed,0);
			boost = 1;
		}
		vspeed = nprop_set(vspeed,-SHIP_SPEED);
	}else{
		vspeed = nprop_set(vspeed,0);
	}
	}
	if (key_pressed('y') && (nprop_get(vspeed) < 700 && nprop_get(vspeed) > - 700 &&
				nprop_get(hspeed) < 700 && nprop_get(hspeed) > -700) ){
		vspeed = nprop_set(vspeed,1.5*nprop_get(vspeed));
		hspeed = nprop_set(hspeed,1.5*nprop_get(hspeed));
	}

	
	particle_set_nprop(p,SHIP_VSPEED,vspeed);
	particle_set_nprop(p,SHIP_HSPEED,hspeed);
	p->v = vec_new(nprop_get(hspeed),nprop_get(vspeed));
	if(boost){
		trail = factory_create_v(p->box.pos,P_SHIP_BOOST);
		trail->box = box_direct(trail->box,p->box.axis0);
	}
	if(vec_len(p->v) > 10.0){
		p->box = box_direct(p->box,p->v);
		p->vector[MISSILE] = p->v;
		trail =  factory_create_v(p->box.pos,P_SHIP_TRAIL);
		trail->box = p->box;
		if(key_pressed('y')){
			particle_set_color(trail,1,1,1,0.3);
			particle_set_alt_color(trail,1,1,1,0.3);
		}
	}
	if(key_pressed('g') && !key_pressed('y') &&  get_time() > p->time[MISSILE]){
		p->time[MISSILE] = get_time() + 1;
		missile = factory_create_v(p->box.pos,P_MISSILE);
		missile->v = vec_add(p->vector[MISSILE],p->v);
		missile->v = vec_add(missile->v,spread);
		missile->die_time = get_time() + random()%1000+1000;
		particle_set_solid(p,0);
	}
	if(!key_pressed('g')){
		particle_set_solid(p,1);
	}
	if(key_pressed('r') && get_time() > p->time[MISSILE]){
		trail = factory_create_v(p->box.pos,P_WALL);
		trail = factory_create_v(vec_add(p->box.pos,vec_new(10,-10)),P_BG);
		p->time[MISSILE] = get_time() + 500;
	}
	if(key_pressed('f')){
		p->box = box_rotate(p->box,1);
	}
}
static void ship_collide(particle_t *p, particle_t*s){
	if(key_pressed('y')){
		particle_set_nprop(p,SHIP_VSPEED,nprop_set_hard(particle_get_nprop(p,SHIP_VSPEED),-nprop_get(particle_get_nprop(p,SHIP_VSPEED))));
		particle_set_nprop(p,SHIP_HSPEED,nprop_set_hard(particle_get_nprop(p,SHIP_HSPEED),-nprop_get(particle_get_nprop(p,SHIP_HSPEED))));
	}
}
static void ship_draw(particle_t *p){
	float angle = vec_angle(p->box.axis0);
	/*particle_draw_square(p);*/
	model_draw_shadow(p->model[0],p->box.pos.x-5,p->box.pos.y-5,-50,16.0,angle - 90);
	model_draw(p->model[0],p->box.pos.x,p->box.pos.y,0,15.0,angle - 90);

	
}
static void register_ship(void){
	particle_t*p;
	model_t *mod;
	material_t *mat;

	mod = model_load("data/ship.obj");
	
	mat = material_new();
	material_set_diffuse(mat,1,0,0,1.0);
	material_set_spec(mat,1,0.5,0.2,1.0);
	material_set_edge(mat,0.5,0,0,0.1);
	material_set_shininess(mat,50);
	material_enable(mat, DRAW_FACE | DRAW_EDGE);
	model_set_material(mod,1,mat);

	mat = material_new();
	material_set_diffuse(mat,0.8,0.8,0.8,1);
	material_set_spec(mat,0.8,0.8,0.8,1);
	material_set_shininess(mat,50);
	material_set_edge(mat,0,0,0,0.1);
	material_enable(mat, DRAW_FACE | DRAW_EDGE);
	model_set_material(mod,0,mat);

	mat = material_new();
	material_set_diffuse(mat,0,0,0,1);
	material_set_edge(mat,1,1,1,0.1);
	material_enable(mat, DRAW_FACE | DRAW_EDGE );
	model_set_material(mod,2,mat);


	p = particle_new(box_new(vec_new(0,0),40,32),0);
	p->draw = ship_draw;	
	p->move = particle_simple_move;
	p->action = ship_action;
	p->collide = ship_collide;
	p->vector[MISSILE] = vec_new(100,0);
	p->model[0] = mod;
	particle_set_color(p,1,0.3,0,0.1);
	particle_set_alt_color(p,1,0.5,0,0.5);
	particle_set_collides(p,1);
	particle_set_camera(p,1);
	particle_set_solid(p,1);
	particle_set_group(p,1);
	particle_set_nprop(p,SHIP_HSPEED,nprop_new(0,SHIP_ACCEL));
	particle_set_nprop(p,SHIP_VSPEED,nprop_new(0,SHIP_ACCEL));
	factory_register(p,P_SHIP);	
}
/*------------------------------------------*\
 * SHIP GFX 
\*------------------------------------------*/

static void trail_action(particle_t *p){
	p->color[3] *= 0.93;
	p->color[2] *= 0.85;
	p->color[1] *= 0.5;
	p->altcolor[3] *= 0.9;
	p->altcolor[2] *= 0.7;
	p->altcolor[1] *= 0.5;
	p->param[0] = p->param[0] -1;
	p->box = box_rotate(p->box,random()%11 - 5);
	if(p->param[0] < 0 ){
		p->life = -1;
	}
	return;
}
static void boost_action(particle_t *p){
	p->color[3] *= 0.93;
	p->color[0] *= 0.99;
	p->altcolor[3] *= 0.9;
	p->altcolor[0] *= 0.95;
	p->param[0] = p->param[0] -1;
	p->box = box_offset(p->box,5);
	if(p->param[0] < 0 ){
		p->life = -1;
	}
	return;
}
static void register_ship_gfx(void){
	particle_t *p;
	
	p = particle_new(box_new(vec_new(0,0),24,32),-0.5);
	p->draw = particle_draw_square;
	p->action = trail_action;
	p->param[0] = 100;
	particle_set_color(p,1,0.1,0,0.1);
	particle_set_alt_color(p,1,0,0,0.2);
	factory_register(p,P_SHIP_TRAIL);

	p = particle_new(box_new(vec_new(0,0),4,4),8);
	p->draw = particle_draw_square;
	p->action = boost_action;
	p->param[0] = 100;
	particle_set_color(p,1,0.1,0,0.1);
	particle_set_alt_color(p,1,0,0,0.2);
	factory_register(p,P_SHIP_BOOST);
}
/*------------------------------------------*\
 * SHIP MISSILE 
\*------------------------------------------*/

static void missile_die(particle_t*p){
	particle_t *missile = NULL;
	vec_t newdir = vec_new(random()%256 - 128, random()%256 - 128);
	missile = factory_create_v(p->box.pos,P_MISSILE);
	missile->v = vec_add(newdir,vec_scale(p->v,0.7));;
	missile->die_time = get_time() + random()%1000 + 250;
	missile->box = box_rotate(missile->box,random()%360);
	if(random()%100 <1){
		missile->die = NULL;
	}
}
static void missile_collide(particle_t *p,particle_t*s){
	if(!s->move){
		particle_set_color(p,1,0.1,0,0.1);
		particle_set_alt_color(p,1,0,0,0.1);
	}else{
		particle_set_color(p,0.5,1,0,0.1);
		particle_set_alt_color(p,0.9,1,0,0.3);
	}
}
static void register_ship_missile(void){
	particle_t *p;

	p = particle_new(box_new(vec_new(0,0),10,10),2);
	particle_set_color(p,0,0.5,1,0.1);
	particle_set_alt_color(p,0.1,0.6,1,0.1);
	p->draw = particle_draw_square;
	p->move = particle_simple_move;
	p->collide = missile_collide;
	particle_set_collides(p,1);
	p->a = vec_new(0,0);
	particle_set_solid(p,0);
	p->die = missile_die;
	factory_register(p,P_MISSILE);
}

void game_register_ship(void){
	register_ship();
	register_ship_gfx();
	register_ship_missile();
}
