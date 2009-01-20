#include <stdlib.h>
#include <string.h> 
#include <stdio.h> 
#include <GL/glut.h>
#include "model.h"

#define MAX_VERT 50000
#define LINE_LENGTH 256
#define NO_VERT -1

float v_buffer[MAX_VERT*3];
int v_buffer_count;

int f_buffer[MAX_VERT*4];
int f_buffer_count;

/*---------------------------*\
 * GEOMETRY AND OBJ LOADING
\*---------------------------*/
static void vbuffer_clear(void){
	v_buffer_count = 0;
}
static void vbuffer_append(float x, float y, float z){
	if(v_buffer_count < MAX_VERT){
		v_buffer[v_buffer_count*3    ] = x;
		v_buffer[v_buffer_count*3 + 1] = y;
		v_buffer[v_buffer_count*3 + 2] = z;
		v_buffer_count++;
	}else{
		printf("ERROR : Model has too much vertices (max:%d)\n",MAX_VERT);
	}
}
static void fbuffer_clear(void){
	f_buffer_count = 0;
}
static void fbuffer_append(int v0, int v1, int v2, int v3){
	if(v_buffer_count < MAX_VERT){
		f_buffer[f_buffer_count*4    ] = v0;
		f_buffer[f_buffer_count*4 + 1] = v1;
		f_buffer[f_buffer_count*4 + 2] = v2;
		f_buffer[f_buffer_count*4 + 3] = v3;
		f_buffer_count++;
	}else{
		printf("ERROR : Model has too much faces (max:%d)\n",MAX_VERT);
	}
}
static face_group_t *face_group_new(void){
	face_group_t *fg = (face_group_t*)malloc(sizeof(face_group_t));
	fg->face_count = f_buffer_count;
	fg->material = NULL;
	fg->face = (int*)malloc(f_buffer_count*4*sizeof(int));
	memcpy(fg->face,f_buffer,f_buffer_count*4*sizeof(int));
	return fg;
}
static int readline(char *buffer, FILE*file, int length){
	char c;
	int i = 0;
	while(i < length){
		c = (char)fgetc(file);
		buffer[i] = c;
		if( i == length -1){
			buffer[i] = '\0';
			printf("ERROR: readline() : line too long\n");
			return 0;
		}else if( c == EOF){
			buffer[i] = '\0';
			return 0;
		}else if (c == '\n'){
			buffer[i] = '\0';
			return 1;
		}
		i++;
	}
	return 0;
}
model_t *model_load(const char *path){
	FILE*f = NULL;
	char line[LINE_LENGTH];
	char *lptr = NULL;
	char *sptr = NULL;
	float x,y,z;
	int v1,v2,v3,v4;
	model_t *m;

	vbuffer_clear();
	fbuffer_clear();
	f = fopen(path,"r");
	/* READING HEADER */
	if(f == NULL){
		printf("ERROR: model_load(): file not found:%s\n",path);
	}
	if(!readline(line,f,LINE_LENGTH)){
		printf("ERROR: model_load(): error reading file :\"%s\"\n",path);
		return NULL;
	}
	while(readline(line,f,LINE_LENGTH) && line[0] != 'o');
	/* READING VERTEX*/
	while(readline(line,f,LINE_LENGTH)){
		if (line[0] == 'v'){
			x = (float)strtod(line + 2,&lptr);
			y = (float)strtod(lptr, &lptr);
			z = (float)strtod(lptr, &sptr);
			if(lptr == sptr){
				printf("ERROR: model_load(): wrong vertex format in file : %s\n",path);
				return NULL;
			}
			vbuffer_append(x,y,z);
		}else{
			break;
		}
	}
	m = (model_t*)malloc(sizeof(model_t));
	m->vertex_count = v_buffer_count;
	m->group_count = 0;
	m->vertex = (float*)malloc(m->vertex_count*3*sizeof(float));
	memcpy(m->vertex,v_buffer,m->vertex_count*3*sizeof(float));
	
	/* READING FACES */	
	fbuffer_clear();
	do{
		if(!strncmp(line,"usemtl",6) && f_buffer_count != 0){
			if(m->group_count == MAX_FACE_GROUP){
				printf("ERROR: model_load(): too much face groups in file :%s\n",path);
				fbuffer_clear();
			}else{
				m->group[m->group_count] = face_group_new();
				m->group_count++;
				fbuffer_clear();
				printf("buffer saved\n");
			}
		}else if(line[0] == 'f'){
			v1 = (int)strtol(line + 1,&lptr,10) -1;
			v2 = (int)strtol(lptr,&lptr,10) - 1;
			v3 = (int)strtol(lptr,&lptr,10) - 1;
			v4 = (int)strtol(lptr,&sptr,10) - 1;
			if(lptr == sptr){
				v4 = NO_VERT;
			}
			fbuffer_append(v1,v2,v3,v4);
		}
	}while(readline(line,f,LINE_LENGTH));
	if(f_buffer_count != 0){
		if(m->group_count == MAX_FACE_GROUP){
			printf("ERROR: model_load(): too much face groups in file :%s\n",path);
			fbuffer_clear();
		}else{
			m->group[m->group_count] = face_group_new();
			m->group_count++;
			fbuffer_clear();
		}
	}
	return m;
}
void	model_set_material(model_t *m, int index, material_t *mat){
	if(index < MAX_FACE_GROUP && m->group[index] != NULL){
		m->group[index]->material = mat;
	}else{
		printf("ERROR: model_set_material(), invalid material index :%d\n", index);
	}
}
static void face_group_print(face_group_t *fg){
	int i = 0;
	printf("FACE_GROUP\n");
	printf("\tface_count:%d\n",fg->face_count);
	printf("\tFACES:\n");
	while(i < fg->face_count){
		printf(		"\t\t%d\t%d\t%d",
				fg->face[4*i],
				fg->face[4*i + 1],
				fg->face[4*i + 2]
				);
		if(fg->face[4*i + 3] != NO_VERT){
			printf("\t%d\n",fg->face[4*i + 3]);
		}else{
			printf("\n");
		}
		i++;
	}
}
void model_print(model_t* m){
	int i = 0;
	printf("MODEL:\n");
	printf("\tvertex_count: %d\n",m->vertex_count);
	printf("\tgroup_count: %d\n",m->group_count);
	printf("\tVERTEX:\n");
	while(i < m->vertex_count){
		printf(		"\t\t%f\t%f\t%f\n",
				m->vertex[3*i],
				m->vertex[3*i + 1],
				m->vertex[3*i + 2]	);
		i++;
	}
	printf("\tFACE GROUPS:\n");
	i = 0;
	while(i < m->group_count){
		face_group_print(m->group[i]);
		i++;
	}
}


/*---------------------------*\
 * MATERIALS
\*---------------------------*/


material_t *material_new(void){
	material_t *m = (material_t*)malloc(sizeof(material_t));
	memset(m,0,sizeof(material_t));
	m->users = 0;
	return m;
}
void material_set_diffuse(material_t *mat,float r,float g, float b, float a){
	mat->color_diffuse[0] = r;
	mat->color_diffuse[1] = g;
	mat->color_diffuse[2] = b;
	mat->color_diffuse[3] = a;
}
void material_set_spec(material_t *mat,float r,float g, float b, float a){
	mat->color_spec[0] = r;
	mat->color_spec[1] = g;
	mat->color_spec[2] = b;
	mat->color_spec[3] = a;
}
void material_set_emit(material_t *mat,float r,float g, float b, float a){
	mat->color_emit[0] = r;
	mat->color_emit[1] = g;
	mat->color_emit[2] = b;
	mat->color_emit[3] = a;
}
void material_set_edge(material_t *mat,float r,float g, float b, float a){
	mat->color_edge[0] = r;
	mat->color_edge[1] = g;
	mat->color_edge[2] = b;
	mat->color_edge[3] = a;
}
void material_enable(material_t *mat, int drawmode){
	mat->drawmode |= drawmode;
}
void material_disable(material_t *mat, int drawmode){
	mat->drawmode &= !drawmode;
}

/*---------------------------*\
 * MATERIALS
\*---------------------------*/

static void draw_polygon(model_t *m, face_group_t* fg, int mode ){
	int j,k,v;
	float x,y,z;
	j = 0;
	while(j < fg->face_count){
		glBegin(mode);
		k = 0;
		while(k < 4){
			if((v=fg->face[4*j + k])!= NO_VERT){
				x = m->vertex[3*v];
				y = m->vertex[3*v + 1];
				z = m->vertex[3*v + 2];
				glVertex3f(x,y,z);	
			}
			k++;
		}

		glEnd();
		j++;
	}
}
void model_draw(model_t *m, float x, float y, float z, float scale, float angle){
	int i = 0;
	face_group_t *fg;
	material_t *mat;
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x,y,z);
	glScalef(scale,scale,scale);
	glRotatef(angle,0,0,1);
	while(i < m->group_count){
		fg = m->group[i];
		mat = fg->material;
		if(mat == NULL){
			return;
		}
		if(mat->drawmode & DRAW_FACE){
			glColor4f(	mat->color_diffuse[0],
					mat->color_diffuse[1],
					mat->color_diffuse[2],
					mat->color_diffuse[3]	);
			draw_polygon(m,fg,GL_POLYGON);
		}
		if(mat->drawmode & DRAW_EDGE){
			glColor4f(	mat->color_edge[0],
					mat->color_edge[1],
					mat->color_edge[2],
					mat->color_edge[3]	);
			draw_polygon(m,fg,GL_LINE_LOOP);
		}
		if(mat->drawmode & DRAW_VERTEX){
			glPointSize(3);
			glColor4f(	mat->color_edge[0],
					mat->color_edge[1],
					mat->color_edge[2],
					mat->color_edge[3]	);
			draw_polygon(m,fg,GL_POINTS);
		}
		i++;
	}
	
	glPopMatrix();
}

	

