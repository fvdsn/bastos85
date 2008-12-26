#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include "particle.h"
#include "virtual_time.h"
#include "keyboard.h"
particle_t *particle_new(box_t box, float z){
	particle_t *p = (particle_t*)malloc(sizeof(particle_t));
	memset(p,0,sizeof(particle_t));
	p->box = box;
	p->z = z;
	p->think_interval = 100;
	p->life = 1;
	return p;
}
void particle_free(particle_t*p){
	free(p);
}
int particle_is_dead(const particle_t*p){
	return p->life <= 0;
}
void particle_kill(particle_t*p){
	p->life = -1;
}
void particle_set_solid(particle_t*p, int trueorfalse){
	p->solid = trueorfalse;
}
int particle_is_solid(particle_t*p){
	return p->solid;
}
void particle_set_flag(particle_t*p, int id, int trueorfalse){
	int flag = 1 << id;
	if(trueorfalse){
		p->flags |= flag;
	}else{
		p->flags &= ~flag;
	}
}
int particle_get_flag(const particle_t*p,int id){
	return (p->flags & (1 << id)) != 0;
}
void particle_set_param(particle_t *p, int id, float param){
	if(id >= 0 && id < PARTICLE_PARAM_COUNT){
		p->param[id] = param;
	}
}
float particle_get_param(particle_t*p, int id){
	if(id >= 0 && id < PARTICLE_PARAM_COUNT){
		return p->param[id];
	}else{
		return 0.0;
	}
}
void particle_set_color(particle_t*p,float r, float g, float b, float a){
	p->color[0] = r;
	p->color[1] = g;
	p->color[2] = b;
	p->color[3] = a;
}
void particle_set_lifetime(particle_t*p, vmsec_t time){
	p->die_time = get_time() + time;
}
void particle_set_camera(particle_t *p, int trueorfalse){
	p->camera = trueorfalse;
}
int particle_is_camera(particle_t *p){
	return p->camera;
}
int particle_z_sort(const void *a, const void *b){
	particle_t *pa = *(particle_t**)a;
	particle_t *pb = *(particle_t**)b;
	if(pa->z < pb->z){
		return -1;
	}else if (pa->z == pb->z){
		return 0;
	}else{
		return 1;
	}
}
void particle_draw_console(particle_t*self){
	printf("particle:");
	printf("\tpos(%f,%f)\tv(%f,%f)\ta(%f,%f)\n",
			self->box.pos.x,
			self->box.pos.y,
			self->v.x,
			self->v.y,
			self->a.x,
			self->a.y);
}
void particle_simple_move(particle_t *self){
	float dtime = get_dtime_sec();
	self->v = vec_add(self->v,vec_scale(self->a,dtime));
	self->box.pos = vec_add(self->box.pos,vec_scale(self->v,dtime));
}
void particle_simple_think(particle_t *self){
	float tmp = self->color[0];
	self->color[0] = self->color[1];
	self->color[1] = self->color[2];
	self->color[2] = tmp;
}
void particle_simple_action(particle_t *self){
	int speed = 300;
	if(key_pressed('a')){
		self->v.x = -speed;
	}
	if(key_pressed('d')){
		self->v.x = speed;
	}
	if(!key_pressed('a') && !key_pressed('d')){
		self->v.x = 0;
	}
	if(key_pressed('s')){
		self->v.y = -speed;
	}
	if(key_pressed('w')){
		self->v.y = speed;
	}
	if(!key_pressed('s') && !key_pressed('w')){
		self->v.y = 0;
	}
}





