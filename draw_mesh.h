#ifndef _DRAW_MATRIX_H
#define _DRAW_MATRIX_H

#include "matrix_vector.h" 
#include "structs.h"
#include "draw.h"
#include <math.h>

int setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    int color = 0;
    
    color |= (r << 24);     // Set red component
    color |= (g << 16);     // Set green component
    color |= (b << 8);      // Set blue component
    color |= a;             // Set alpha component
    return color;
}


void draw_mesh(game_t* game,object_t* object,int r,int g,int b){
    mat4x4_t mat_rotz, mat_rotx,mat_roty;
    mat_rotz = matrix_make_rotation_z(object->rotation_z);
    mat_roty = matrix_make_rotation_y(object->rotation_y);
    mat_rotx = matrix_make_rotation_x(object->rotation_x);
    mat4x4_t mat_trans;
    mat_trans = matrix_make_translation(object->x, object->y, object->z);
    mat4x4_t mat_world;
    mat_world = matrix_make_identity();
    mat_world = matrix_multiply_matrix(&mat_rotz, &mat_rotx);
    mat_world = matrix_multiply_matrix(&mat_world, &mat_roty);
    mat_world = matrix_multiply_matrix(&mat_world, &mat_trans);

    vec3_t v_up = {0,1,0};
    vec3_t v_target = {0,0,1};//vector_add(&game->vcamera,&game->look_dir);
    mat4x4_t mat_camera_rot = matrix_make_rotation_y(game->f_yaw);
    game->look_dir = matrix_multiply_vector(&mat_camera_rot,&v_target);
    v_target = vector_add(&game->vcamera,&game->look_dir);
    mat4x4_t mat_camera = matrix_point_at(&game->vcamera,&v_target,&v_up);
    mat4x4_t mat_view = matrix_quick_inverse(&mat_camera);
    for (int i = 0; i < object->mesh->length; i++)
    {
        shape_triangle_t tri_projected, tri_transformed, tri_viewed;
        tri_transformed.p[0] = matrix_multiply_vector(&mat_world, &object->mesh->tris[i].p[0]);
        tri_transformed.p[1] = matrix_multiply_vector(&mat_world, &object->mesh->tris[i].p[1]);
        tri_transformed.p[2] = matrix_multiply_vector(&mat_world, &object->mesh->tris[i].p[2]);

        vec3_t normal, line1, line2;
        line1 = vector_sub(&tri_transformed.p[1], &tri_transformed.p[0]);
        line2 = vector_sub(&tri_transformed.p[2], &tri_transformed.p[0]);

        // Take cross product of lines to get normal to triangle surface
        normal = cross_product(&line1, &line2);

        // You normally need to normalise a norm
        normal = normalize(&normal);
        vec3_t vcamera_ray = vector_sub(&tri_transformed.p[0], &game->vcamera);
        // if(normal.z>0) continue; //skip
        if (dot_product(&normal, &vcamera_ray) < 0.0f)
        {

            // Illumintaion
            vec3_t light_direction; //!light is kinda meh but its fine for now
            extract_forward_direction(&mat_camera,&light_direction);
            light_direction.y += normalize(&game->vcamera).y;
            //light_direction = normalize(&light_direction);
            float dp = MAX(0.1f, dot_product(&light_direction, &normal));
            //world -> view
            tri_viewed.p[0] = matrix_multiply_vector(&mat_view,&tri_transformed.p[0]);
            tri_viewed.p[1] = matrix_multiply_vector(&mat_view,&tri_transformed.p[1]);
            tri_viewed.p[2] = matrix_multiply_vector(&mat_view,&tri_transformed.p[2]);
            tri_viewed.color = setRGBA(dp *r,dp *g,dp *b,0);
            int nclipped_triangles = 0;
            shape_triangle_t clipped[2] = {default_triangle,default_triangle};
            nclipped_triangles = triangle_clip_against_plane((vec3_t){ 0.0f, 0.0f, 0.1f },(vec3_t){ 0.0f, 0.0f, 1.0f }, &tri_viewed, &clipped[0], &clipped[1]);

            for (int n = 0; n<nclipped_triangles; n++)
            {
            // 3D -> 2D

                tri_projected.p[0] = matrix_multiply_vector(&game->mat_proj, &clipped[n].p[0]);
                tri_projected.p[1] = matrix_multiply_vector(&game->mat_proj, &clipped[n].p[1]);
                tri_projected.p[2] = matrix_multiply_vector(&game->mat_proj, &clipped[n].p[2]);
                tri_projected.color = clipped[n].color; // ADD COLOR
                tri_projected.p[0] = vector_div(&tri_projected.p[0], tri_projected.p[0].w);
                tri_projected.p[1] = vector_div(&tri_projected.p[1], tri_projected.p[1].w);
                tri_projected.p[2] = vector_div(&tri_projected.p[2], tri_projected.p[2].w);
                
                tri_projected.p[0].x *= -1.0f;
                tri_projected.p[1].x *= -1.0f;
                tri_projected.p[2].x *= -1.0f;

                tri_projected.p[0].y *= -1.0f;
                tri_projected.p[1].y *= -1.0f;
                tri_projected.p[2].y *= -1.0f;
                
                vec3_t offset_view = {1, 1, 0};
                tri_projected.p[0] = vector_add(&tri_projected.p[0], &offset_view);
                tri_projected.p[1] = vector_add(&tri_projected.p[1], &offset_view);
                tri_projected.p[2] = vector_add(&tri_projected.p[2], &offset_view);

                // scale into view
                tri_projected.p[0].x *= 0.5f * (float)screen_width;
                tri_projected.p[0].y *= 0.5f * (float)screen_height;
                tri_projected.p[1].x *= 0.5f * (float)screen_width;
                tri_projected.p[1].y *= 0.5f * (float)screen_height;
                tri_projected.p[2].x *= 0.5f * (float)screen_width;
                tri_projected.p[2].y *= 0.5f * (float)screen_height;


                //test
                /*
                tri_projected.p[0].x = round(tri_projected.p[0].x);
                tri_projected.p[0].y = round(tri_projected.p[0].y);
                tri_projected.p[1].x = round(tri_projected.p[1].x);
                tri_projected.p[1].y = round(tri_projected.p[1].y); 
                tri_projected.p[2].x = round(tri_projected.p[2].x);
                tri_projected.p[2].y = round(tri_projected.p[2].y);*/

                stack_push(game->sort_verts,tri_projected);
            }
        }
    }
}
    //printf("%d\n",mesh.verts_sorted_length);
    // bubble_sort(mesh.verts_sorted,mesh.verts_sorted_length);
    //printf("i have survived3\n");


int draw_sky(game_t* game){
    for (int i = 0; i<screen_width; i++)
        for (int j = 0; j<screen_height; j++)
            draw_pixel(i,j,0,0,MAX(MIN((j+(int)game->vcamera.y+30),255),0));


}
#endif