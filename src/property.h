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
 * @param vp : a vector property.
 * @return : the current value of the vprop
 */
vec_t   vprop_get(vprop_t vp);
/**
 * Sets the target of the vprop (property will be animated
 * until it reach that value )
 * @param vp : a vector property
 * @param v : a vector
 * @return a new vprop with same content as vp but with v as target
 */
vprop_t vprop_set(vprop_t vp, vec_t v);
/**
 * Sets the animation speed of a vector propery.
 * @param vp : the vprop
 * @param s : the speed (in unit/sec) of animation
 * @return a new prop with same content as vp but with the new
 * animation speed. If speed is 0 : it will not be animated.
 */
vprop_t vprop_set_speed(vprop_t vp, float s);
/**
 * Animates a vector property.
 * @param vp : the vprop to animate
 * @param sec_dt : the animation duration in sec.
 * @return a new vprop with the value of vp after sec_dt
 * of animation.
 */
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
