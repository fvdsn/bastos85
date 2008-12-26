#include <stdio.h> 
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"
vec_t cam_pos;
int win_w, win_h;

float bg_color[4] = {0,0,0,1};
void draw_init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);*/
}
static void draw_redraw(void){
	glViewport(0,0, (GLsizei)win_w, (GLsizei)win_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(	-win_w/2.0 +cam_pos.x,
			win_w/2.0  +cam_pos.x, 
			-win_h/2.0 +cam_pos.y, 
			win_h/2.0  +cam_pos.y,
			-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void draw_reshape(int w, int h){
	win_w = w;
	win_h = h;
	draw_redraw();
}
void draw_background(void){
	glClear(GL_COLOR_BUFFER_BIT);
}
void particle_draw_square(particle_t*self){
	vec_t ul = box_upperleft(self->box);
	vec_t dr = box_downright(self->box);
	glColor3f(	self->color[0],
			self->color[1],
			self->color[2]);
	glBegin(GL_POLYGON);
		glVertex3f(ul.x,ul.y,0.0);
		glVertex3f(dr.x,ul.y,0.0);
		glVertex3f(dr.x,dr.y,0.0);
		glVertex3f(ul.x,dr.y,0.0);
	glEnd();
}
void draw_flush(void){
	glutSwapBuffers();
}
void camera_set_position(vec_t pos){
	cam_pos = pos;
	draw_redraw();
}
void camera_set_zone(vec_t pos, float freezone,float elastic){
	vec_t zone = vec_scale(vec_new(win_w/2,win_h/2),freezone);
	vec_t newpos = cam_pos;
	int lx = cam_pos.x - zone.x;
	int rx = cam_pos.x + zone.x;
	int by = cam_pos.y - zone.y;
	int ty = cam_pos.y + zone.y;
	if(pos.x < lx){
		newpos.x -= (lx - pos.x)*elastic;
	}
	else if(pos.x > rx){
		newpos.x += (pos.x - rx)*elastic;
	}
	if(pos.y < by){
		newpos.y -= (by - pos.y)*elastic;
	}else if (pos.y > ty){
		newpos.y += (pos.y - ty)*elastic;
	}
	camera_set_position(newpos);
}

vec_t camera_get_position(void){
	return cam_pos;
}

void background_set_color(float r, float g, float b, float alpha){
	bg_color[0] = r;
	bg_color[1] = g;
	bg_color[2] = b;
	bg_color[3] = alpha;
}

