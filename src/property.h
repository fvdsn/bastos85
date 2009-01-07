#ifndef __2D_PROPERTY_H__
#define __2D_PROPERTY_H__
#include "vector.h"
#include "virtual_time.h"

/**
 * Vector Property : Allows to animate a vector in a simple manner
 */
typedef struct vprop_s{
	/** Always the current value of the vector */
	vec_t val;
	/** The speed (unit/seconds) of the vector modification */
	float speed;
	/** The final value of the animation */
	vec_t target;
}vprop_t;
/**
 * Creates a new vector property with a current value
 * The vprop is not animated initially.
 * @param val : the current value of the vector property
 */
vprop_t vprop_new(vec_t val);
/**
 * Return the current value of the vprop
 * @return : the current value of the vprop
 */
vec_t   vprop_get(vprop_t vp);
vprop_t vprop_set(vprop_t vp, vec_t v);
vprop_t vprop_set_speed(vprop_t vp, float s);
vprop_t vprop_animate(vprop_t vp, float sec_dt);

typedef struct nprop_s{
	float val;
	float speed;
	float target;
	float max;
}nprop_t;

nprop_t nprop_new(float val,float speed);
float   nprop_get(nprop_t np);
nprop_t nprop_set(nprop_t np, float val);
nprop_t nprop_set_speed(nprop_t np, float s);
nprop_t nprop_set_max(nprop_t np,float max);
nprop_t nprop_animate(nprop_t np, float dt);
nprop_t nprop_animate_mod(nprop_t np, float sec_dt);

#endif
