#include <stdio.h> 
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"
void draw_init(){
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
}
void draw_reshape(int w, int h){
	glViewport(0,0, (GLsizei)w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2.0, w/2.0, -h/2.0, h/2.0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void draw_background(){
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
void draw_flush(){
	glutSwapBuffers();
}
