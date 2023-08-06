//------------------------YouTube-3DSage----------------------------------------
// Full video: https://www.youtube.com/watch?v=w0Bm4IA-Ii8
// WADS to move player, E open door after picking up the key

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include "init.h"
#include "draw.h"
#include "gmlib/strings.h"
#include "sort.h"
#include <SDL2/SDL_thread.h>
#include "queue.h"
#include "./gmlib/stack.h"
#include "matrix_vector.h"
#include "draw_mesh.h"
mesh_t *load_from_object_file(char *name)
{
    FILE *fptr;
    char line[128];
    fptr = fopen(name, "r");
    int verts_length = 0;
    int mesh_length = 0;
    int verts_size = 0;
    int shape_size = 0;
    while (fgets(line, sizeof(line), fptr))
    {
        if (line[0] == 'v')
            verts_size++;
        if (line[0] == 'f')
            shape_size++;
    }
    
    fclose(fptr);
    vec3_t *verts = calloc(sizeof(vec3_t) * verts_size, 1);
    mesh_t *finished_mesh = calloc(sizeof(mesh_t), 1);

    finished_mesh->tris = calloc(sizeof(shape_triangle_t) * shape_size, 1);
    //finished_mesh->verts_sorted = calloc(sizeof(shape_triangle_t) * shape_size*3, 1); //!TEMP STUFF NOT SUPPOSED TO BE 3

    finished_mesh->length = shape_size;
    fptr = fopen(name, "r");

    while (fgets(line, sizeof(line), fptr))
    {
        if (line[0] == 'v' || line[0] == 'f')
        {
            string_t *temp = c_string(line);
            int start = 0;
            int verts_num = 0;
            int flats_num = 0;
            char last_char = 0;
            for (int i = 0; temp->string[i] != 0; i++)
            {

                if ((temp->string[i] == '\t' || temp->string[i] == ' ' || temp->string[i] == '\n' || temp->string[i + 1] == '\0') && (temp->string[i + 1]!=' ' && temp->string[i + 1]!='\t'))
                {
                    string_t *token;
                    if (temp->string[i + 1] != '\0')
                        token = copy_string(temp, start, i);
                    else
                        token = copy_string(temp, start, i + 1);

                    if (line[0] == 'v')
                    {
#ifdef debug
                        if (verts_num == 0)
                            printf("v ");
                        else
                            printf("%f ", atof(token->string));
#endif
                        if (verts_num == 1)
                            verts[verts_length].x = atof(token->string);
                        if (verts_num == 2)
                            verts[verts_length].y = atof(token->string);
                        if (verts_num == 3)
                        {
                            verts[verts_length].z = atof(token->string);
                            verts[verts_length].w = 1; // default
                            verts_length++;
#ifdef debug
                            printf("\n");
#endif
                        }
                        verts_num++;
                    }
                    if (line[0] == 'f')
                    {
#ifdef debug
                        if (flats_num == 0)
                            printf("f ");
                        else
                            printf("%d ", atoi(token->string));
#endif
                        if (flats_num == 1)
                            finished_mesh->tris[mesh_length].p[0] = verts[atoi(token->string) - 1];
                        if (flats_num == 2)
                            finished_mesh->tris[mesh_length].p[1] = verts[atoi(token->string) - 1];
                        if (flats_num == 3)
                        {
                            finished_mesh->tris[mesh_length].p[2] = verts[atoi(token->string) - 1];
                            mesh_length++;
#ifdef debug
                            printf("\n");
#endif
                        }

                        flats_num++;
                    }
                    start = i + 1;
                }
            }
        }
    }
    fclose(fptr);
    printf("triss %d %d %d %d", verts_length, verts_size, shape_size, mesh_length);
    return finished_mesh;
}

// ################################################### DISPLAY ################################################
float roate_test = 0;


const vec3_t plane_normals[4] = {
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, -1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f }
};

const vec3_t plane_points[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, (float)screen_height - 1, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { (float)screen_width - 1, 0.0f, 0.0f }
};

//shape_triangle_t clipped[2] = {default_triangle,default_triangle};
void display(game_t *game)
{

    draw_sky(game);
    draw_mesh(game,&game->objects[0],100,100,0);
    
    draw_mesh(game,&game->objects[1],0,255,0);
    draw_mesh(game,&game->objects[2],155,155,0);
    

    game->objects[2].rotation_z=sinf(SDL_GetTicks()*0.01)/12;
    game->objects[2].rotation_y=cosf(SDL_GetTicks()*0.01)/12;
    game->objects[2].x=cosf(SDL_GetTicks()*0.01)/12;
    game->objects[1].rotation_z=sinf(SDL_GetTicks()*0.01);

    quicksort(game->sort_verts->data, 0, game->sort_verts->top - 1);
    

    shape_triangle_t clipped[2];
    shape_triangle_t* clipped_ptr = clipped; // Pointer to the clipped array
    for (int i = 0; i < game->sort_verts->top; i++)
    {
        
        
    stack_push(game->triangle_clip_stack,game->sort_verts->data[i]);
    int nnew_triangles = 1;
    for (int p = 0; p < 4; p++) {
    while (nnew_triangles-- > 0) {
        shape_triangle_t test = stack_pop(game->triangle_clip_stack);

        int ntris_to_add = triangle_clip_against_plane(plane_points[p], plane_normals[p], &test, &clipped[0], &clipped[1]);

        for (int w = 0; w < ntris_to_add; ++w)
            stack_push(game->triangle_clip_stack, clipped[w]);
    }
    nnew_triangles = game->triangle_clip_stack->top;
    if (nnew_triangles==0) break;

    }

    shape_triangle_t tri;
    while (game->triangle_clip_stack->top>0)
    {
        tri = stack_pop(game->triangle_clip_stack);
        fill_triangle(tri.p[0].x, tri.p[0].y,
                    tri.p[1].x, tri.p[1].y,
                    tri.p[2].x, tri.p[2].y,
                    (tri.color >> 24) & 0xFF,
                    (tri.color>> 16) & 0xFF,
                    (tri.color>> 8) & 0xFF);


    }
    
    }

    game->sort_verts->top = 0;
    //fill_triangle2(20, 20, 30, 40, 70, 70, 200, 200, 100);
}
          /* draw_triangle(tri.p[0].x, tri.p[0].y,
                      tri.p[1].x, tri.p[1].y,
                      tri.p[2].x, tri.p[2].y,100,255,100);*/

void game_keyboard(game_t *game)
{
while(SDL_PollEvent(&game->event)){
switch(game->event.type){
    // look for a keypress
    case SDL_QUIT: game->state = 0; break;
    case SDL_KEYDOWN:
            game->keys[game->event.key.keysym.scancode] = true;
        break;
    case SDL_KEYUP:
        game->keys[game->event.key.keysym.scancode] = false;
    break; 
    }
}
    //}
}


void handle_input(game_t* game){
    if(game->keys[SDL_SCANCODE_UP])
    game->vcamera.y+=8*delta_time;

    if(game->keys[SDL_SCANCODE_DOWN])
    game->vcamera.y-=8*delta_time;

    if(game->keys[SDL_SCANCODE_RIGHT])
    game->vcamera.x-=8*delta_time;

    if(game->keys[SDL_SCANCODE_LEFT])
    game->vcamera.x+=8*delta_time;

    vec3_t v_forward = vector_mul(&game->look_dir,8*delta_time);

    if(game->keys[SDL_SCANCODE_W])
        game->vcamera = vector_add(&game->vcamera,&v_forward);

    if(game->keys[SDL_SCANCODE_S])
        game->vcamera = vector_sub(&game->vcamera,&v_forward);

    if(game->keys[SDL_SCANCODE_A])
    game->f_yaw-=7*delta_time;

    if(game->keys[SDL_SCANCODE_D])
    game->f_yaw+=7*delta_time;
}

#define TRIANGLE(mesh0, num, x0, y0, z0, x1, y1, z1, x2, y2, z2) ({ \
    (mesh0).tris[num].p[0].x = (x0);                                \
    (mesh0).tris[num].p[0].y = (y0);                                \
    (mesh0).tris[num].p[0].z = (z0);                                \
    (mesh0).tris[num].p[0].w = 1;                                    \
    (mesh0).tris[num].p[1].x = (x1);                                \
    (mesh0).tris[num].p[1].y = (y1);                                \
    (mesh0).tris[num].p[1].z = (z1);                                \
    (mesh0).tris[num].p[1].w = 1;                                   \
    (mesh0).tris[num].p[2].x = (x2);                                \
    (mesh0).tris[num].p[2].y = (y2);                                \
    (mesh0).tris[num].p[2].z = (z2);                                \
    (mesh0).tris[num].p[2].w = 1;                                   \
    (mesh0).tris[num].color = 1;                                    \
})

int main(int argc, char *argv[])
{
    // test mesh
    mesh_t cube_mesh;
    
    cube_mesh.length = 6 * 2; // 6 sides in cube * 2 triangles per side
    cube_mesh.tris = calloc(sizeof(shape_triangle_t) * cube_mesh.length, 1);
    
    // SOUTH
    TRIANGLE(cube_mesh, 0, 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f);
    TRIANGLE(cube_mesh, 1, 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f);
    // EAST
    TRIANGLE(cube_mesh, 2, 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f);
    TRIANGLE(cube_mesh, 3, 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f);
    // NORTH
    TRIANGLE(cube_mesh, 4, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f);
    TRIANGLE(cube_mesh, 5, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f);
    // WEST
    TRIANGLE(cube_mesh, 6, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f);
    TRIANGLE(cube_mesh, 7, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f);
    // TOP
    TRIANGLE(cube_mesh, 8, 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f);
    TRIANGLE(cube_mesh, 9, 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f);
    // BOTTOM
    TRIANGLE(cube_mesh, 10, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f);
    TRIANGLE(cube_mesh, 11, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f);
    
       for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            default_matrix.m[c][r]=0;


    // test
    game_t *game = setup();
    game->mat_proj = matrix_make_projection(90.0f, (float)screen_height / (float)screen_width, 0.1f, 1000.0f);
    game->objects[0] = (object_t){mesh : load_from_object_file("./models/mountains.obj"), x: 0,y : -.2, z: 0.3,
    rotation_x:0,rotation_y:0,rotation_z:0};

    game->objects[1] = (object_t){mesh : &cube_mesh, x: 10,y : 12, z: 8,
    rotation_x:0,rotation_y:0,rotation_z:0};

    game->objects[2] = (object_t){mesh : load_from_object_file("./models/cow.obj"), x: 20,y : 12, z: 4,
    rotation_x:0,rotation_y:0,rotation_z:0};
    char text[20];
    int timer = 0;
    uint32_t start_time, frame_time;
    float fps;
    float average_fps = 30.0f;
    //performance test
    float startTime = 0;
    float endTime = 1;
    printf("\n");
    vec3_t tvar = {0,0,0,0}; 
    float avg = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    /*

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_custom_functions(L);
    if ( luaL_dofile(L,"./lua/main.lua")!=0){
        lua_error(L);
    }

    lua_getglobal(L,"setup");
    if ( lua_pcall(L,0,0,0)!=0){
        lua_error(L);
    }*/


    while (game->state != 0)
    {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

        start_time = SDL_GetTicks();
        delta_time = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency() );
        draw_start();
            game_keyboard(game);
            handle_input(game);
           // lua_getglobal(L,"loop");
           // if ( lua_pcall(L,0,0,0)!=0){
             //   lua_error(L);
            //}

            display(game);
        
        draw_end();

       

        
        frame_time = SDL_GetTicks()-start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;
        //average_fps = (average_fps + fps)/2;
        average_fps = 0.9 * average_fps + (1.0 - 0.9) * fps;
        if (timer>=50){
        sprintf(text,"fps : %.2f",fps);
        SDL_SetWindowTitle(game->window,text);
        timer=0;
        }
        timer++;
    }
    printf("Average FPS : %.4f",average_fps);
}