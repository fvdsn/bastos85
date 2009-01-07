#include <stdio.h>
#include "property.h"
#include "vector.h"
#include "virtual_time.h"


vprop_t vprop_new(vec_t val){
	vprop_t vp;
	vp.val = val;
	return vp;
}
vec_t   vprop_get(vprop_t vp){
	return vp.val;
}
vprop_t vprop_set(vprop_t vp, vec_t v){
	if(vp.speed == 0.0 ){
		vp.val = v;
	}
	vp.target = v;
	return vp;
}
vprop_t vprop_set_speed(vprop_t vp, float s){
	vp.speed = s;
	return vp;
}
vprop_t vprop_animate(vprop_t vp, float dt){
	vec_t s;
	if (!(vp.speed == 0.0 || vec_equal(vp.val,vp.target))){
		s = vec_scale(vec_normalize(vec_diff(vp.val,vp.target)),vp.speed);
		if(vec_dist(vp.val,vp.target) < vec_len(s)){
			vp.val = vp.target;
		}else{
			vp.val = vec_add(vp.val,s);
		}
	}
	return vp;
}

/*---------------------------*\
 * Numeric properties
\*---------------------------*/ 
nprop_t nprop_new(float val, float speed){
	nprop_t np;
	np.val = val;
	np.speed = speed;
	return np;
}
float   nprop_get(nprop_t np){
	return np.val;
}
nprop_t nprop_set(nprop_t np, float val){
	if(np.max){
		while(val < 0 ){
			val += np.max;
		}
		while(val > np.max){
			val -= np.max;
		}

	}
	if(np.speed == 0){
		np.val = val;
	}
	np.target = val;
	return np;

}
nprop_t nprop_set_speed(nprop_t np, float s){
	if(s < 0){
		s = 0;
	}
	np.speed = s;
	return np;
}
nprop_t nprop_set_max(nprop_t np,float max){
	np.max = max;
	while(np.val < 0){
		np.val += max;
	}
	while(np.val > max){
		np.val -= max;
	}
	while(np.target < 0){
		np.target += max;
	}
	while(np.target > max){
		np.target -= max;
	}
	return np;
}
nprop_t nprop_animate(nprop_t np, float dt){
	float s;
	if(!(np.speed == 0 || np.val == np.target)){
		s = np.speed*dt;
		/*printf("%f\n",s);*/
		if(np.val > np.target){
			if(np.val - np.target < s){
				np.val = np.target;
			}else{
				np.val -=s;
			}
		}else{
			if(np.target - np.val < s){
				np.val = np.target;
			}else{
				np.val += s;
			}
		}
		
	}
	/*printf("[v:%f s:%f t:%f]\n",np.val,np.speed,np.target);*/
	return np;
}
nprop_t nprop_animate_mod(nprop_t np, float dt){
	return np;
}

