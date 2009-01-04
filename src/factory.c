#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "factory.h"
#include "world.h"
#include "vector.h"

particle_t *klass_table[MAX_KLASS_COUNT];

void factory_register(particle_t *p, int klass){
	if(klass < 0 || klass >= MAX_KLASS_COUNT){
		return;
	}
	if(klass_table[klass]!= NULL){
		particle_free(klass_table[klass]);
	}
	klass_table[klass] = p;
}
particle_t *factory_create_v(vec_t pos, int klass){
	particle_t *p = NULL;
	if(klass < 0 || klass >= MAX_KLASS_COUNT || klass_table[klass] == NULL){
		return NULL;
	}else{
		p = particle_new(box_new(pos,10,10),0);
		memcpy(p,klass_table[klass],sizeof(particle_t));
		p->box.pos = pos;
		world_add_particle(world_get(),p);
		return p;
	}
}
particle_t *factory_create(float x, float y, int klass){
	return factory_create_v(vec_new(x,y),klass);
}
particle_t *factory_resized_v(vec_t tl, vec_t tr, int klass){
	vec_t pos = vec_scale(vec_add(tl,tr),0.5);
	vec_t size = vec_abs(vec_diff(pos,tr));
	particle_t *p = factory_create_v(pos,klass);
	if(p!=NULL){
		p->box.size = size;
	}
	return p;
}
particle_t *factory_resized(float blx, float bly, float rtx, float rty, int klass){
	return factory_resized_v(vec_new(blx,bly),vec_new(rtx,rty),klass);
}

