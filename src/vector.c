#include <math.h>
#include "vector.h"
#define RADIAN(X)	((X)/180.0)*3.141592
vec_t vec_new(float x, float y){
	vec_t v;
	v.x = x;
	v.y = y;
	return v;
}
vec_t vec_polar(float angle, float length){
	return vec_rotate(vec_new(length,0),angle);
}
vec_t  vec_add(vec_t a,vec_t b){
	a.x += b.x;
	a.y += b.y;
	return a;
}
vec_t vec_inv(vec_t a){
	a.x = -a.x;
	a.y = -a.y;
	return a;
}
vec_t vec_abs(vec_t a){
	if(a.x < 0){
		a.x = -a.x;
	}
	if(a.y < 0){
		a.y = -a.y;
	}
	return a;
}
vec_t vec_scale(vec_t a, float fact){
	a.x *= fact;
	a.y *= fact;
	return a;
}
vec_t vec_normalize(vec_t a){
	if (vec_zero(a)){
		return a;
	}else{
		return vec_scale(a,1.0/vec_len(a));
	}
}
vec_t vec_rotate(vec_t a, float angle){
	float sina = sinf(RADIAN(angle));
	float cosa = cosf(RADIAN(angle));
	return vec_add(	vec_new(a.x*cosa,  a.y*sina),
			vec_new(-a.y*sina, a.y*cosa)  );

}
vec_t vec_diff(vec_t a, vec_t b){
	b.x -= a.x;
	b.y -= a.y;
	return b;
}
vec_t vec_project(vec_t a,vec_t b){
	float d = vec_dot(a,b)
	float l = b.x*b.x + b.y*b.y;
	if(l == 0.0){
		return vec_new(0,0);
	}else{
		d /= l;
		return vec_new(d*b.x, d*b.y);
	}
}
vec_t vec_perp(vec_t a){
	return vec_new(-a.y,a.x);
}
vec_t vec_smallest(vec_t a, vec_t b){
	float alen = a.x*a.x + a.y*b.y;
	float blen = b.x*b.x + b.y*b.y;
	if(alen < blen){
		return a;
	}else{
		return b;
	}
}
float vec_dot(vec_t a, vec_t b){
	return a.x*b.x + a.y*b.y;
}
/**
 * Returns the absolute value of the doc product
 */
static float vec_absdot(vec_t a,vec_t b){
	float d = a.x*b.x + a.y*b.y;
	if(d < 0){
		return - d;
	}else{
		return d;
	}
}
float vec_len(vec_t a){
	return sqrtf(a.x*a.x + a.y*a.y);
}
float vec_dist(vec_t a,vec_t b){
	return vec_len(vec_diff(a,b));
}
int vec_zero(vec_t a){
	return (a.x == 0) && (a.y == 0);
}
box_t box_new(vec_t p, float width, float height){ 
	box_t b;
	b.pos = p;
	b.size = vec_abs(vec_new(width/2.0,height/2.0));
	b.axis0 = vec_new(1,0);
	b.axis1 = vec_new(0,1);
	return b;
}
int box_intersect(box_t a, box_t b){
	vec_t d = vec_abs(vec_diff(a.pos, b.pos));
	return 	(d.x < a.size.x + b.size.x) &&
		(d.y < a.size.y + b.size.y);
}
static box_t box_rotate(box_t b, float angle){
	b.axis0 = vec_rotate(b.axis0,angle);
	b.axis1 = vec_rotate(b.axis1,angle);
	return b;
}
vec_t box_intersect_vector(box_t a, box_t b){
	vec_t d = vec_diff(a.pos, b.pos);
	vec_t r = vec_new(0,0);
	if (d.x >= 0){
		r.x = (b.pos.x - b.size.x) - (a.pos.x + a.size.x);
		if ( r.x > 0)
			r.x = 0;
	}else{
		r.x =  (b.pos.x + b.size.x) - (a.pos.x - a.size.x);
		if (r.x < 0)
			r.x = 0;
	}
	if (d.y >= 0){
		r.y = (b.pos.y - b.size.y) - (a.pos.y + a.size.y);
		if ( r.y > 0)
			r.y = 0;
	}else{
		r.y =  (b.pos.y + b.size.y) - (a.pos.y - a.size.y);
		if (r.y < 0)
			r.y = 0;
	}
	d = vec_abs(r);
	if(d.x > d.y){
		r.x = 0;
	}else{
		r.y = 0;
	}
	return r;
}
vec_t box_upperleft(box_t a){
	return vec_add(a.pos,vec_scale(a.size,-1.0));
}
vec_t box_downright(box_t a){
	return vec_add(a.pos,a.size);
}
/**
 * Collision detection of two box with arbitrary angles. Does
 * only one side (a->b) of the collision. 
 * if it returns no collision -> no collision
 * if it returns a collision -> check the other side (b<-a)
 * It will return a vector to displace a so that there is no collision
 * anymore. the vector returned is one of the normals of b.
*/
static vec_t box_o_o_collision(box_t a, box_t b){
	/* we will perform the collision using b point of view and axis 
	 * because those will be considered the surface normals, and we
	 * want to return 'a' displacement in terms of 'b' surface normals 
	 * and not in terms of 'a' orientation vectors. 
	 * 	that's why the logic looks reverse at times 
	 * */
	vec_t dist = vec_diff(b.pos,a.pos);
	/* normalized axis of the box b. One of those axis will be the
	 * surface normal. */
	vec_t axn[2] = {b.axis0,b.axis1};

	/*distance we need to displace a to remove the collision. 
	 * (0,0) => no collision*/
	float[2] col_vec = {0,0};
	int i = 2;
	while(i--){
		/* we check if the distance from center to center is smaller
		 * than the sum of the distance from center to box edges.
		 * and that projected along each vector normal. 
		 * If it is false for one normal, we have no collision. 
		 * otherwise we have the displacement distance to remove 
		 * the collision.
		 */
		col_vec[i] =      vec_absdot(axn[i],a.haxis0) 
				+ vec_absdot(axn[i],a.haxis1)
				- vec_absdot(axn[i],dist);	
		if(col_vec[i] <= 0){	/* No intersection ! */
			return vec_new(0,0);
		}
	}
	/* col_vec has the distance we need to displace 'a' along axn[0],axn[1]
	 * to remove the collision. We now need to pick the shortest 
	 * distance and the right direction.
	 * This is just choosing smallest dist and flipping normals if 
	 * necessary.
	 */
	if(col_vec[0] < col_vec[1]){
		if(vec_dot(dist,axn[0]) > 0 ){
			return vec_scale(axn[0], col_vec[0]);
		}else{
			return vec_scale(axn[0],-col_vec[1]);
		}
	}else{
		if(vec_dot(dist,axn[1]) > 0){
			return vec_scale(axn[1], col_vec[1]);
		}else{
			return vec_scale(axn[1],-col_vec[1]);
		}
	}
}
vec_t box_oriented_collision(box_t a, box_t b){
	vec_t d1;
	vec_t d2;
	d1 = box_o_o_collision(a,b);
	if(vec_zero(d1)){
		return d1;
	}else{
		d2 = vec_inv(box_o_o_collision(b,a));
		if(vec_zero(d2)){
			return d2;
		}else{
			return vec_smallest(d1,d2);
		}
	}
}

