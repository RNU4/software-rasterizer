//Global varibels for drawing
#ifndef _INIT_H
#define _INIT_H
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
/*
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
*/
#include "gmlib/config.h"
#include "./gmlib/stack.h"

#define COLOR_MAX 255
/*##############-> Macros <-##############*/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#pragma GCC target("avx,avx2,fma")
#pragma GCC optimization ("unroll-loops")

#define bench_start(start_time)({ \
start_time = clock(); \
})
#define bench_end(start_time)({ \
double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;\
printf("Done in %f seconds\n", elapsed_time);\
})

//globals
game_t* game;
void *pixels;
Uint8 *base;
int pitch;
SDL_Texture *texture;
mat4x4_t default_matrix;
double delta_time = 0;
//shape_triangle_t default_triangle = {.p[0].w = 1, .p[1].w = 1, .p[2].w = 1};
#define default_triangle (const shape_triangle_t){.p[0].w = 1, .p[1].w = 1, .p[2].w = 1}

game_t* setup(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    game_t* game;
    game=(game_t*)calloc(sizeof(game_t),1);
    game->state = 1;
    game->window = 
    SDL_CreateWindow("GAME", // creates a window
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    screen_width, screen_height, SDL_WINDOW_RESIZABLE   );

    for(int i = 0; i < 322; i++) { // init them all to false
        game->keys[i] = false;
    }
    game->sort_verts = stack_new(512);
    game->triangle_clip_stack = stack_new(8);
    uint32_t render_flags = SDL_RENDERER_ACCELERATED ;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"),
    //SDL_RenderSetIntegerScale(game->renderer, SDL_TRUE),
    game->renderer = SDL_CreateRenderer(game->window, -1, render_flags);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    SDL_RenderSetLogicalSize(game->renderer, screen_width, screen_height);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);
    texture = NULL;
    texture = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
    
    IMG_Init(IMG_INIT_PNG);
    return game;
}
#endif