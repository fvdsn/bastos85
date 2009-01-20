#ifndef __2D_MODEL_H__
#define __2D_MODEL_H__

#define MAX_FACE_GROUP 32

#define DRAW_FACE 1
#define DRAW_EDGE 2
#define DRAW_VERTEX 4
#define DRAW_TRANSP 8

typedef struct material_s{
	float color_diffuse[4];
	float color_spec[4];
	float color_emit[4];
	float color_edge[4];
	float shininess;
	int drawmode;
	int users;
}material_t;

typedef struct face_group_s{
	int face_count;
	int *face;
	int *normal;
	material_t *material;
}face_group_t;


typedef struct model_s{
	int vertex_count;
	int group_count;
	float *vertex;
	float *normal;
	face_group_t *group[MAX_FACE_GROUP];
}model_t;

model_t *model_load(const char *path);
void	model_set_material(model_t *m, int index, material_t *mat);
void 	 model_draw(model_t *m, float x, float y, float z, float scale, float angle);
void 	 model_draw_shadow(model_t *m, float x, float y, float z, float scale, float angle);
void	model_print(model_t *m);
material_t *material_new(void);
void	material_set_diffuse(material_t *mat,float r,float g, float b, float a);
void	material_set_spec(material_t *mat,float r,float g, float b, float a);
void	material_set_emit(material_t *mat,float r,float g, float b, float a);
void	material_set_edge(material_t *mat,float r,float g, float b, float a);
void	material_set_shininess(material_t *mat, float s);
void	material_enable(material_t *mat, int drawmode);
void	material_disable(material_t *mat, int drawmode);

#endif
