#ifndef _MATRIX_VECTOR_H
#define _MATRIX_VECTOR_H

// ################################################### VECTOR ################################################
#include "structs.h"
#include "init.h"
#include <math.h>
float dot_product(vec3_t *v1, vec3_t *v2)
{
    return v1->x*v2->x + v1->y*v2->y + v1->z * v2->z;
}

vec3_t cross_product(vec3_t *v1, vec3_t *v2)
{
    return (vec3_t){
        x : v1->y * v2->z - v1->z * v2->y,
        y : v1->z * v2->x - v1->x * v2->z,
        z : v1->x * v2->y - v1->y * v2->x,
        w : 1
    };
}


float vector_length(vec3_t *v)
{
    return sqrtf(dot_product(v, v));
}

vec3_t normalize(vec3_t *v)
{
    float len = vector_length(v);
    return (vec3_t){x : (v->x / len), y : (v->y / len), z : (v->z / len),w:1};
}

vec3_t vector_add(vec3_t *v1, vec3_t *v2)
{
    return (vec3_t){x:v1->x + v2->x,
                    y:v1->y + v2->y, 
                    z:v1->z + v2->z,
                    w:1};
}
vec3_t vector_sub(vec3_t *v1, vec3_t *v2)
{
    return (vec3_t){x:v1->x - v2->x,
                    y:v1->y - v2->y,
                    z:v1->z - v2->z,
                    w:1};
}

vec3_t vector_mul(vec3_t *v1, float k)
{
    return (vec3_t){x:v1->x * k, 
                    y:v1->y * k, 
                    z:v1->z * k,
                    w:1};
}

vec3_t vector_div(vec3_t *v1, float k)
{
    return (vec3_t){x:v1->x / k,
                    y:v1->y / k,
                    z:v1->z / k,
                    w:1};
}

// ################################################### MATRIX ################################################
vec3_t matrix_multiply_vector(mat4x4_t *m, vec3_t *i)
{
    vec3_t v;
    v.x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + i->w * m->m[3][0];
    v.y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + i->w * m->m[3][1];
    v.z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + i->w * m->m[3][2];
    v.w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + i->w * m->m[3][3];
    return v;
}

mat4x4_t matrix_make_identity()
{
    mat4x4_t matrix=default_matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrix_make_rotation_x(float angle)
{
    mat4x4_t matrix=default_matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(angle);
    matrix.m[1][2] = sinf(angle);
    matrix.m[2][1] = -sinf(angle);
    matrix.m[2][2] = cosf(angle);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrix_make_rotation_y(float angle)
{
    mat4x4_t matrix=default_matrix;
    matrix.m[0][0] = cosf(angle);
    matrix.m[0][2] = sinf(angle);
    matrix.m[2][0] = -sinf(angle);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(angle);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrix_make_rotation_z(float angle)
{
    mat4x4_t matrix=default_matrix;
    matrix.m[0][0] = cosf(angle);
    matrix.m[0][1] = sinf(angle);
    matrix.m[1][0] = -sinf(angle);
    matrix.m[1][1] = cosf(angle);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrix_make_translation(float x, float y, float z)
{
    mat4x4_t matrix=default_matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

mat4x4_t matrix_make_projection(float f_fov, float f_asepc, float f_near, float f_far)
{
    float f_fov_rad = 1.0f / tanf(f_fov * 0.5f / 180.0f * 3.14159f);
    mat4x4_t matrix=default_matrix;
    matrix.m[0][0] = f_asepc * f_fov_rad;
    matrix.m[1][1] = f_fov_rad;
    matrix.m[2][2] = f_far / (f_far - f_near);
    matrix.m[3][2] = (-f_far * f_near) / (f_far - f_near);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

mat4x4_t matrix_multiply_matrix(mat4x4_t *m1, mat4x4_t *m2)
{
    mat4x4_t matrix=default_matrix;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
    return matrix;
}


mat4x4_t matrix_point_at(vec3_t* pos,vec3_t* target, vec3_t *up){
    
    //New forward
    vec3_t new_forward = vector_sub(target,pos);
    new_forward = normalize(&new_forward);
    //new up direction
    vec3_t a = vector_mul(&new_forward,dot_product(up,&new_forward));
    vec3_t new_up = vector_sub(up,&a);
    new_up = normalize(&new_up);

    //new right
    vec3_t new_right = cross_product(&new_up,&new_forward);


        // Construct Dimensioning and Translation Matrix	
    mat4x4_t matrix = default_matrix;
    matrix.m[0][0] = new_right.x;	matrix.m[0][1] = new_right.y;	matrix.m[0][2] = new_right.z;	matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = new_up.x;		matrix.m[1][1] = new_up.y;		matrix.m[1][2] = new_up.z;		matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = new_forward.x;	matrix.m[2][1] = new_forward.y;	matrix.m[2][2] = new_forward.z;	matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = pos->x;			matrix.m[3][1] = pos->y;			matrix.m[3][2] = pos->z;			matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrix_quick_inverse(mat4x4_t* m) // Only for Rotation/Translation Matrices
{
    mat4x4_t matrix = default_matrix;
    matrix.m[0][0] = m->m[0][0]; matrix.m[0][1] = m->m[1][0]; matrix.m[0][2] = m->m[2][0]; matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m->m[0][1]; matrix.m[1][1] = m->m[1][1]; matrix.m[1][2] = m->m[2][1]; matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m->m[0][2]; matrix.m[2][1] = m->m[1][2]; matrix.m[2][2] = m->m[2][2]; matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m->m[3][0] * matrix.m[0][0] + m->m[3][1] * matrix.m[1][0] + m->m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m->m[3][0] * matrix.m[0][1] + m->m[3][1] * matrix.m[1][1] + m->m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m->m[3][0] * matrix.m[0][2] + m->m[3][1] * matrix.m[1][2] + m->m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    return matrix;
}


//!SNAKE CASE IT
vec3_t vector_intersect_plane(vec3_t* plane_p, vec3_t* plane_n, vec3_t* lineStart, vec3_t* lineEnd)
{
    *plane_n = normalize(plane_n);
    float plane_d = -dot_product(plane_n, plane_p);
    float ad = dot_product(lineStart, plane_n);
    float bd = dot_product(lineEnd, plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    vec3_t lineStartToEnd = vector_sub(lineEnd, lineStart);
    vec3_t lineToIntersect = vector_mul(&lineStartToEnd, t);
    return vector_add(lineStart, &lineToIntersect);
}

int triangle_clip_against_plane(vec3_t plane_p, vec3_t plane_n, shape_triangle_t* in_tri, shape_triangle_t* out_tri1, shape_triangle_t* out_tri2)
{
    plane_n = normalize(&plane_n);
	#define dist(p)(plane_n.x * (p).x + plane_n.y * (p).y + plane_n.z * (p).z - dot_product(&plane_n, &plane_p))
    	// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		vec3_t* inside_points[3];  int nInsidePointCount = 0;
		vec3_t* outside_points[3]; int nOutsidePointCount = 0;

		// Get signed distance of each point in triangle to plane

		float d0 = dist(in_tri->p[0]);
		float d1 = dist(in_tri->p[1]);
		float d2 = dist(in_tri->p[2]);

        if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri->p[0]; }
		else { outside_points[nOutsidePointCount++] = &in_tri->p[0]; }
		if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri->p[1]; }
		else { outside_points[nOutsidePointCount++] = &in_tri->p[1]; }
		if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri->p[2]; }
		else { outside_points[nOutsidePointCount++] = &in_tri->p[2]; }
		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist
			return 0; // No returned triangles are valid
		}
		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			out_tri1->p[0] = in_tri->p[0];
            out_tri1->p[1] = in_tri->p[1];
            out_tri1->p[2] = in_tri->p[2];
            out_tri1->color = in_tri->color;

			return 1; // Just the one returned original triangle is valid
		}
		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1->color =  in_tri->color;

			// The inside point is valid, so keep that...
			out_tri1->p[0] = *inside_points[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			out_tri1->p[1] = vector_intersect_plane(&plane_p, &plane_n, inside_points[0], outside_points[0]); //!idk seems kinda sus
			out_tri1->p[2] = vector_intersect_plane(&plane_p, &plane_n, inside_points[0], outside_points[1]);

			return 1; // Return the newly formed single triangle
		}

        if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1->color =  in_tri->color;

			out_tri2->color =  in_tri->color;
			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1->p[0] = *inside_points[0];
			out_tri1->p[1] = *inside_points[1];
			out_tri1->p[2] = vector_intersect_plane(&plane_p, &plane_n, inside_points[0], outside_points[0]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2->p[0] = *inside_points[1];
			out_tri2->p[1] = out_tri1->p[2];
			out_tri2->p[2] = vector_intersect_plane(&plane_p, &plane_n, inside_points[1], outside_points[0]);

			return 2; // Return two newly formed triangles which form a quad
		}

}

void extract_forward_direction(mat4x4_t* camera_matrix, vec3_t* forward) {
    forward->x = -camera_matrix->m[2][0];
    forward->y = -camera_matrix->m[2][1];
    forward->z = -camera_matrix->m[2][2];
    *forward = normalize(forward);
}

#endif