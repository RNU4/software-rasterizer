#ifndef _STRUCTS_H
#define _STRUCTS_H
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct vec3
{
    float x, y, z, w; /* 3d vec */
} vec3_t;

typedef struct shape_triangle
{
    vec3_t p[3]; /* points */
    int color;
} shape_triangle_t;


typedef struct mesh
{
    shape_triangle_t *tris;
    //shape_triangle_t *verts_sorted;
    //int verts_sorted_length;
    int length;
} mesh_t;


typedef struct object{
    /* data */
    mesh_t* mesh;
    float x;
    float y;
    float z;
    
    float rotation_x;
    float rotation_y;
    float rotation_z;

}object_t;


typedef struct mat4x4
{
    /* data */
    float m[4][4];
} mat4x4_t;

typedef struct stack{
    int top;
    int size;
    shape_triangle_t* data;
} stack_t;


typedef struct game{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event; //keyboard input 
    bool keys[322];  // 322 is the number of SDLK_DOWN events
    int state;
    mat4x4_t mat_proj;
    vec3_t vcamera;
    vec3_t look_dir;
    stack_t* sort_verts;
    stack_t* triangle_clip_stack;
    object_t objects[10];

    float f_yaw;

}game_t;

#endif