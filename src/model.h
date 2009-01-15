#ifndef __2D_MODEL_H__
#define __2D_MODEL_H__

typedef struct material_s{
	float color_diffuse[4];
	float color_spec[4];
	float color_emit[4];
	float color_edge[4];
	int drawmode;
	int users;
}material_t;

typedef struct face_group_s{
	int face_count;
	int *face;
	material_t *m;
}face_group_t;


typedef struct model_s{
	int vertex_count;
	int group_count;
	float *vertex;
	face_group_t **group;
}model_t;

model_t *model_load(char *path);
void	model_set_material(model_t *m, material_t *m);
void 	 model_draw(model_t *m, vec_t pos);

#endif
