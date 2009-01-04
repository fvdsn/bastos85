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
 * Creates a new vector with polar coordinates.
 * @param angle: the angle in degrees of the vector. 0 is East, 90 is North
 * @param length: the length of the vector.
 */
vec_t  vec_polar(float angle, float length);
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
 * Normalizes a vector
 * @param a vector
 * @return: a vector with length 1 and the same direction. 
 * if a is zero then return vector zero.
 */
vec_t vec_normalize(vec_t a);
/** 
 * rotate the vector counterclockwise 
 * @param a : a vector.
 * @param angle : angle in ]-360,360[ rotation in degrees.
 * @return : the vector rotated by angle.
 */
vec_t vec_rotate(vec_t a, float angle);
/**
 * Returns the vector from a to b
 * @param a : the source vector
 * @param b : the destination vector
 * @return : a vector c such as a + c = b
 */
vec_t vec_diff(vec_t a, vec_t b);
/**
 * Projects a vector onto another
 * @param a : the vector to be projected
 * @param b : the target vector
 * @return : the projection of a on b, or (0,0) if b is (0,0)
 */
vec_t vec_project(vec_t a, vec_t b);
/**
 * Returns a vector perpendicular to a, of the same length as a. 
 * @param a : a vector
 * @return a vector perpendicular to a.
 */
vec_t vec_perp(vec_t a);
/**
 * Returns the vector with the smallest length.
 * Far more efficient than comparing vec_len.
 * @param a : a vector
 * @param b : a vector
 * @return a if it's len is smaller than b's len, b otherwise.
 */
vec_t vec_smallest(vec_t a, vec_t b);
/**
 * Dot product between vector a and b
 * @param a : a vector
 * @param b : a vector
 */
float vec_dot(vec_t a, vec_t b);
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
 * Return the angle of the vector.
 * @param a : a vector.
 * @return 0 if the vector is zero, else x in [0,360[ where
 * x is the angle of the vector. 0 is East, 90 is North
 */
float vec_angle(vec_t a);
/**
 * Returns 1 if a is the zero vector, 0 otherwize
 */
int vec_zero(vec_t a);

/**
 * A not so fancy box.
 */
typedef struct box_s{
	vec_t pos;	/*the coordinate of the center of the box*/
	vec_t size;	/*the upper right corer coordinates from the center*/
	vec_t axis0;	/*normalized axis. Its orientation gives the angle */
	vec_t axis1;	/*normalized secondary axis, perpendicular to axis0*/
}box_t;
/**
 * Creates a new box.
 * @param pos : the center of the box
 * @param width : the width (WE) of the box
 * @param height : the height (NS) of the box.
 * @return the new box
 */
box_t box_new(vec_t pos, float width, float height);
/**
 * Rotates the box counterclockwise.
 * @param b : the rotating box.
 * @param angle : in ]-360,360[ the rotating angle (ccw) in degrees.
 */
box_t box_rotate(box_t b, float angle);
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
vec_t box_aligned_collision(box_t a, box_t b);
vec_t box_oriented_collision(box_t a, box_t b);

#endif

