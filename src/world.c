#include <string.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <GL/glut.h>
#include "world.h"
#include "draw.h"
#include "particle.h"
#include "virtual_time.h"
#include "keyboard.h"

world_t *current_world = NULL;
world_t *world_get(void){
	return current_world;
}
void 	world_set(world_t *w){
	current_world = w;
}
world_t *world_new(int max_particles){
	world_t *w = (world_t*)malloc(sizeof(world_t));
	w->max_particles = max_particles;
	w->thinking_particle_count = 0;
	w->moving_particle_count   = 0;
	w->visible_particle_count  = 0;
	w->solid_particle_count = 0;
	
	w->all_particle = (particle_t**)malloc(max_particles*sizeof(particle_t*));
	memset(w->all_particle,0,max_particles*sizeof(particle_t*));
	
	w->thinking_particle = (particle_t**)malloc(max_particles*sizeof(particle_t*));
	memset(w->thinking_particle,0,max_particles*sizeof(particle_t*));
	
	w->moving_particle = (particle_t**)malloc(max_particles*sizeof(particle_t*));
	memset(w->moving_particle,0,max_particles*sizeof(particle_t*));
	
	w->visible_particle = (particle_t**)malloc(max_particles*sizeof(particle_t*));
	memset(w->visible_particle,0,max_particles*sizeof(particle_t*));
	
	w->solid_particle = (particle_t**)malloc(max_particles*sizeof(particle_t*));
	memset(w->solid_particle,0,max_particles*sizeof(particle_t*));
	return w;
}
void world_add_particle(world_t *w,particle_t*p){
	int i = 0;
	while(i < w->max_particles){
		if(!w->all_particle[i]){
			w->all_particle[i] = p;
			return;
		}
		i++;
	}
	return;
}
static particle_t *particle_iterator(particle_t**particles,int max_particles,int *id){
	particle_t*p;
	if (id == NULL){
		return NULL;
	}else{
		while( *id < max_particles){
			p = particles[*id];
			if( p == NULL || particle_is_dead(p) ){					
				(*id)++;
			}else{
				(*id)++;
				return p;
			}
		}
		return NULL;
	}
}
particle_t *world_next_particle(world_t*w,int *id){
	return particle_iterator(w->all_particle,w->max_particles,id);
}
particle_t *world_next_solid(world_t*w,box_t range,int *id){
	return particle_iterator(w->solid_particle,w->solid_particle_count,id);
}
particle_t *world_next_drawable(world_t*w,int *id){
	return particle_iterator(	w->visible_particle,
					w->visible_particle_count,
					id);
}
particle_t *world_next_moving(world_t*w,int *id){
	return particle_iterator(w->moving_particle,w->moving_particle_count,id);
}
particle_t *world_next_thinking(world_t*w,int *id){
	return particle_iterator(	w->thinking_particle,
					w->thinking_particle_count,
					id);
}
void world_setup_iterators(world_t*w){
	particle_t *p = NULL;
	int i = 0;
	w->thinking_particle_count = 0;
	w->moving_particle_count = 0;
	w->visible_particle_count = 0;
	w->solid_particle_count = 0;
	while((p=world_next_particle(w,&i))){
		if(p->think || p->action || p->die_time || particle_is_camera(p)){
			w->thinking_particle[w->thinking_particle_count] = p;
			w->thinking_particle_count++;
		}
		if(p->move){
			w->moving_particle[w->moving_particle_count] = p;
			w->moving_particle_count++;
		}
		if(p->draw){
			w->visible_particle[w->visible_particle_count] = p;
			w->visible_particle_count++;
		}
		if(particle_is_solid(p)){
			w->solid_particle[w->solid_particle_count] = p;
			w->solid_particle_count++;
		}
	}
}
void do_world(void){
	world_t *w = world_get();
	world_setup_iterators(w);
	keyboard_frame();
	next_time();
	do_physics(w);
	do_think(w);
	do_graphics(w);
	do_sounds(w);
	do_garbage_collect(w);
	wait_frame();
}
void do_physics(world_t *w){
	int i = 0;
	particle_t *p = NULL;
	int j = 0;
	particle_t *q = NULL;
	box_t b = box_new(vec_new(0,0),vec_new(10,10),0);
	vec_t d = vec_new(0,0);
	while((p = world_next_moving(w,&i))){
		j = 0;
		p->move(p);
		if(particle_collides(p)){
			while((q = world_next_solid(w,b,&j))){
				if(q != p && box_intersect(q->box,p->box)){
					d = box_intersect_vector(p->box,q->box);
					p->box.pos = vec_add(p->box.pos,d);
				}
			}
		}
	}
	/*TODO collisions and damage*/
}
void do_graphics(world_t*w){
	int i = 0;
	particle_t *p = NULL;
	draw_background();
	qsort(	w->visible_particle,
		w->visible_particle_count,
		sizeof(particle_t*),
		particle_z_sort);
	
	while((p = world_next_drawable(w,&i))){
			p->draw(p);
	}
	draw_flush();
}
void do_think(world_t *w){
	int i = 0;
	particle_t *p = NULL;
	while((p = world_next_thinking(w,&i))){
		if (p->action){
			p->action(p);
		}
		if (p->think && (get_time() > p->next_think)){
			p->think(p);
			p->next_think = get_time() + p->think_interval;
			p->think_count++;
		}
		if (p->die_time && (get_time() > p->die_time)){
			particle_kill(p);
		}
		if (particle_is_camera(p)){
			camera_set_zone(p->box.pos,0.05,0.015);
		}
		if (particle_is_dead(p) && p->die){
			p->die(p);
		}
	}
}
void do_sounds(world_t *w){
	return;
}
void do_garbage_collect(world_t*w){
	int i = 0;
	particle_t *p = NULL;
	while(i < w->max_particles){
		p = w->all_particle[i];
		if(p && particle_is_dead(p)){
			particle_free(p);
			w->all_particle[i] = NULL;
		}
		i++;
	}
}
/**
 * Function called when a keyboard key is pressed
 */
static void keyboard_down_func(unsigned char key, int x, int y){
	key_down(key);
	switch(key){
		case 'x':
			set_time_scale(0);
			break;
		case 'p':
			set_time_scale(1.0);
			break;
		case 'n':
			set_time_scale(get_time_scale()*1.10);
			break;
		case 'm':
			set_time_scale(get_time_scale()*0.9);
			break;
		default:
			break;
	}
	return;
}
/**
 * Function called when a keyboard key is released
 */
static void keyboard_up_func(unsigned char key, int x, int y){
	key_up(key);
}

int world_main_loop(int argc, char **argv, world_t *w){
	init_time();
	set_fps(90);
	world_set(w);
	/*printf("coucou2\n");	*/
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(250,250);
	glutInitWindowSize(600,400);
	glutCreateWindow("BASTOS 85");
	draw_init();
	glutDisplayFunc(do_world);
	glutIdleFunc(do_world);
	glutReshapeFunc(draw_reshape);
	/*glutIgnoreKeyRepeat(1);*/
	glutKeyboardFunc(keyboard_down_func);
	glutKeyboardUpFunc(keyboard_up_func);
	glutMainLoop();
	return 0;
}



