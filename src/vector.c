#include "vector.h"

vec_t vec_new(float x, float y){
	vec_t v;
	v.x = x;
	v.y = y;
	return v;
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
vec_t vec_diff(vec_t a, vec_t b){
	b.x -= a.x;
	b.y -= a.y;
	return b;
}
float vec_len(vec_t a){
	vec_t v = vec_abs(a);
	return (v.x > v.y) ? v.x : v.y ;
}
float vec_dist(vec_t a,vec_t b){
	return vec_len(vec_diff(a,b));
}
int vec_zero(vec_t a){
	return (a.x == 0) && (a.y == 0);
}
box_t box_new(vec_t p,vec_t d, float a){ 
	box_t b;
	b.pos = p;
	b.corner = vec_abs(vec_scale(d,0.5));
	b.angle = a;
	return b;
}
int box_intersect(box_t a, box_t b){
	vec_t d = vec_abs(vec_diff(a.pos, b.pos));
	return 	(d.x < a.corner.x + b.corner.x) &&
		(d.y < a.corner.y + b.corner.y);
}
vec_t box_intersect_vector(box_t a, box_t b){
	vec_t d = vec_diff(a.pos, b.pos);
	vec_t r = vec_new(0,0);
	if (d.x >= 0){
		r.x = (b.pos.x - b.corner.x) - (a.pos.x + a.corner.x);
		if ( r.x > 0)
			r.x = 0;
	}else{
		r.x =  (b.pos.x + b.corner.x) - (a.pos.x - a.corner.x);
		if (r.x < 0)
			r.x = 0;
	}
	if (d.y >= 0){
		r.y = (b.pos.y - b.corner.y) - (a.pos.y + a.corner.y);
		if ( r.y > 0)
			r.y = 0;
	}else{
		r.y =  (b.pos.y + b.corner.y) - (a.pos.y - a.corner.y);
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
box_t box_floor(box_t a, int height){
	vec_t *pos = &(a.pos);
	vec_t *corner = &(a.corner);
	if(pos->y < height + corner->y){
		pos->y = height + corner->y;
	}
	return a;
}
vec_t box_upperleft(box_t a){
	return vec_add(a.pos,vec_scale(a.corner,-1.0));
}
vec_t box_downright(box_t a){
	return vec_add(a.pos,a.corner);
}
