#ifndef __2D_VECTOR_H__
#define __2D_VECTOR_H__

/**
 * a basic 2d vector. x is horizontal positive on the right
 * y is vertical, positive up
 */
typedef struct vec_s{
	float x;	/*the x coordinate, right is positive*/
	float y;	/*the y coordinate, up is positive*/
}vec_t;
/**
 * Creates a new vector.
 * @param x : the horizontal coordinate, positive to the right
 * @param y : the vertical coordinate, positive up
 */
vec_t  vec_new(float x, float y);
/**
 * Adds two vectors
 * @return : a vector that is the sum of a and b
 */
vec_t vec_add(vec_t a, vec_t b);
/**
 * Inverse a vector
 * @param a : a vector (x,y)
 * @return :  a vector (-x,-y)
 */
vec_t vec_inv(vec_t a);
/**
 * Makes the vector coordinates positive
 * @param a: a vector (x,y)
 * @return a vector (abs(x),abs(y))
 */
vec_t vec_abs(vec_t a);
/**
 * Multiplies a vector by a scalar
 * @param a : the vector to be multiplied
 * @param factor : the multiplication factor
 * @return a new vector (factor*x, factor*y)
 */
vec_t vec_scale(vec_t a, float factor);
/**
 * Returns the vector from a to b
 * @param a : the source vector
 * @param b : the destination vector
 * @return : a vector c such as a + c = b
 */
vec_t vec_diff(vec_t a, vec_t b);
/**
 * Returns the Norm 0 length of a vector (maximum of its coordinates)
 * @param a : a vector (x,y)
 * @return : abs(max(x,y))
 */
float vec_len(vec_t a);
/**
 * Returns the Norm 0 distance between two vectors
 */
float vec_dist(vec_t a, vec_t b);
/**
 * Returns 1 if a is the zero vector, 0 otherwize
 */
int vec_zero(vec_t a);

/**
 * A not so fancy box.
 */
typedef struct box_s{
	vec_t pos;	/*the coordinate of the center of the box*/
	vec_t corner;	/*the upper right corer coordinates from the center*/
	float angle;	/*the angle of rotation of the box*/
}box_t;
/**
 * Creates a new box.
 * @param pos : the center of the box
 * @param dimensions : a vector where x is the width, and y the height of the
 * box
 * @param angle : the initial angle of the box
 * @return the new box
 */
box_t box_new(vec_t pos, vec_t dimensions, float angle); 
/**
 * Creates a new vector at the upper left corner of the box.
 * @param a : the box
 * @return : the upper left corner
 */
vec_t box_upperleft(box_t a);
/**
 * Creates a new vector at the down right corner of the box.
 * @param a : the box
 * @return : the down right corner
 */
vec_t box_downright(box_t a);
/**
 * Checks if two box are intersecting
 * @param a : a box
 * @param b : another box
 * @return 1 if the box are intersecting, 0 otherwise
 */
int   box_intersect(box_t a, box_t b);
/**
 * Returns the minimum movement a box must do so that
 * it doesn't intersect another box.
 * @param a : the box that must move
 * @param b : the static box
 * @return a vector indicating how much a must move so that
 * it doesn't intersect b. if they do not intersect, 
 * it will return (0,0)
 */
vec_t box_intersect_vector(box_t a, box_t b);
/**
 * Prevents the box from going below a certain height
 * @param a : the box
 * @param height : the minimum height :
 * @return : a new box, the same as the old one if it wasn't below height,
 * a new one just above heihgt if it was below
 */
box_t box_floor(box_t a,int height);

#endif

