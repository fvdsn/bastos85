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
	p->life = 1;
	p->collide_group = ~0;
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
	if(trueorfalse){
		p->physflag |= PHYS_SOLID;
	}else{
		p->physflag &= ~PHYS_SOLID;
	}
}
int particle_is_solid(particle_t*p){
	return p->physflag & PHYS_SOLID;
}
void particle_set_collides(particle_t*p, int trueorfalse){
	if(trueorfalse){
		p->physflag |= PHYS_COLLIDE;
	}else{
		p->physflag &= ~PHYS_COLLIDE;
	}
}
int particle_collides(particle_t*p){
	return p->physflag & PHYS_COLLIDE;
}
int particle_collide_with(particle_t *a, particle_t *b){
	return (1 << b->group) & a->collide_group;
}
void particle_set_group(particle_t*p,int group){
	p->group = group;
}
void particle_toggle_collide_all(particle_t*p){
	p->collide_group ^= ~0;
}
void particle_toggle_collide_group(particle_t*p, int group){
	p->collide_group ^= ( 1 << group );
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
void particle_set_alt_color(particle_t*p,float r, float g, float b, float a){
	p->altcolor[0] = r;
	p->altcolor[1] = g;
	p->altcolor[2] = b;
	p->altcolor[3] = a;
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
void particle_add_timer(particle_t*p, void (*timer)(particle_t*p),vmsec_t ti){
	if(p->ptimer_count < PARTICLE_PARAM_COUNT){
		/* FIXME if(ti < 0){ ti = - ti; } Could be useful if vmsec_t
		 * becomes signed ... */
		p->ptimer[p->ptimer_count].timer_interval = ti;
		p->ptimer[p->ptimer_count].timer = timer;
		p->ptimer[p->ptimer_count].next_time = 0;
		p->ptimer_count++;
	}
}
int  particle_has_timer(particle_t*p){
	return p->ptimer_count;
}
void particle_do_timer(particle_t*p,vmsec_t now){
	int i = p->ptimer_count;
	while(i--){
		if(p->ptimer[i].next_time <= get_time()){
			/*FIXME : timer is called max once per frame ! should
			 * be called once every timer_interval.*/
			p->ptimer[i].next_time = get_time() + p->ptimer[i].timer_interval;
			if(p->ptimer[i].timer){
				p->ptimer[i].timer(p);
			}
		}
	}
}
int	particle_has_prop(particle_t*p){
	return p->has_prop;
}

void    particle_set_nprop(particle_t*p, int id, nprop_t np){
	if(id >=0 && id < PARTICLE_PARAM_COUNT){
		p->nprop[id] =	np;
		p->has_prop = 1;
	}
}
nprop_t particle_get_nprop(particle_t*p, int id){
	if(!(id >=0 && id < PARTICLE_PARAM_COUNT)){
		printf("ERROR: particle_get_nprop(): invalid nprop id :%d\n",id);
	}
	return p->nprop[id];
}
void 	particle_do_nprop(particle_t*p, float dt){
	int i = PARTICLE_PARAM_COUNT;
	while(i--){
		/*TODO do not animate non existing properties*/
		p->nprop[i] = nprop_animate(p->nprop[i],dt);
	}
}
void    particle_set_vprop(particle_t*p, int id, vprop_t vp){
	if(id >=0 && id < PARTICLE_PARAM_COUNT){
		p->vprop[id] =	vp;
		p->has_prop = 1;
	}
}
nprop_t particle_get_vprop(particle_t*p, int id){
	if(!(id >=0 && id < PARTICLE_PARAM_COUNT)){
		printf("ERROR: particle_get_vprop(): invalid nprop id :%d\n",id);
	}
	return p->nprop[id];
}
void 	particle_do_vprop(particle_t*p, float dt){
	int i = PARTICLE_PARAM_COUNT;
	while(i--){
		/*TODO do not animate non existing properties*/
		p->vprop[i] = vprop_animate(p->vprop[i],dt);
	}
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
	self->box = box_rotate(self->box,2.5);
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





