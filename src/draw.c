#include <stdio.h> 
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"
vec_t cam_pos;
int win_w, win_h;

/*vec_t [] circle_coord = {
	{1.0,0.0},
*/	

float bg_color[4] = {0,0,0,1};
static void draw_init_lights(void){
	float mat_specular[] = {1.0,1.0,1.0,1.0};
	float mat_shining[] = {50.0};
	float light_position[] = {5,5,10.0,0.0};
	float light_color[] = {1.0,1.0,1.0,1.0};
	float light_ambiant[] = {0.1,0.1,0.1,1.0};
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shining);
	glMaterialfv(GL_BACK,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_BACK,GL_SHININESS,mat_shining);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_color);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_color);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambiant);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_FLAT);
}
void draw_init(void){
	/*glDisable(GL_DEPTH_TEST);*/
	draw_init_lights();
	glClearColor(	bg_color[0],
			bg_color[1],
			bg_color[2],
			bg_color[3]);
	/*glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);*/
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); /*ADDITIVE BLENDING */
	/*glShadeModel(GL_FLAT);*/
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
			-100,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void draw_reshape(int w, int h){
	win_w = w;
	win_h = h;
	draw_redraw();
}
void draw_background(void){
	glClearColor(	bg_color[0],
			bg_color[1],
			bg_color[2],
			bg_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void particle_draw_square(particle_t*self){
	vec_t h0 = vec_scale(self->box.axis0,self->box.size.x);
	vec_t h1 = vec_scale(self->box.axis1,self->box.size.y);
	float px = self->box.pos.x;
	float py = self->box.pos.y;
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColor4f(	self->color[0],
			self->color[1],
			self->color[2],
			self->color[3]);
	glBegin(GL_POLYGON);
		glNormal3f(0,0,1);
		glVertex3f(px +h0.x +h1.x, py +h0.y +h1.y,0.0);
		glNormal3f(0,0,1);
		glVertex3f(px -h0.x +h1.x, py -h0.y +h1.y,0.0);
		glNormal3f(0,0,1);
		glVertex3f(px -h0.x -h1.x, py -h0.y -h1.y,0.0);
		glNormal3f(0,0,1);
		glVertex3f(px +h0.x -h1.x, py +h0.y -h1.y,0.0);
	glEnd();
	glDisable(GL_LIGHTING);
	glColor4f(	self->altcolor[0],
			self->altcolor[1],
			self->altcolor[2],
			self->altcolor[3]);
	glBegin(GL_LINE_LOOP);
		glVertex3f(px +h0.x +h1.x, py +h0.y +h1.y,0.0);
		glVertex3f(px -h0.x +h1.x, py -h0.y +h1.y,0.0);
		glVertex3f(px -h0.x -h1.x, py -h0.y -h1.y,0.0);
		glVertex3f(px +h0.x -h1.x, py +h0.y -h1.y,0.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(px,py,0.0);
		glVertex3f(px+h1.x,py+h1.y,0.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(px,py,0.0);
		glVertex3f(px+h0.x*0.5,py+h0.y*0.5,0.0);
	glEnd();
}
void particle_draw_point(particle_t*self){
	glColor3f(	self->color[0],
			self->color[1],
			self->color[2]);
	glBegin(GL_POINTS);
		glVertex3f(self->box.pos.x,self->box.pos.y,0.0);
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

