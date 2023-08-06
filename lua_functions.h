
#ifndef _LUA_FUNCTIONS_H
#define _LUA_FUNCTIONS_H
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "font.h"
#include "draw.h"
#include "init.h"

static int l_draw_pixel (lua_State *L) {
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double r = luaL_checknumber(L, 3);
    double g = luaL_checknumber(L, 4);
    double b = luaL_checknumber(L, 5);
    draw_pixel((int)x,(int)y,(int)r,(int)g,(int)b);
    return 0;  /* number of results */
}
static int l_draw_rect (lua_State *L) {
    double x1 = luaL_checknumber(L, 1);
    double y1 = luaL_checknumber(L, 2);
    double x2 = luaL_checknumber(L, 3);
    double y2 = luaL_checknumber(L, 4);
    double r = luaL_checknumber(L, 5);
    double g = luaL_checknumber(L, 6);
    double b = luaL_checknumber(L, 7);
    draw_rect(x1,y1,x2,y2,r,g,b);
    return 0;  /* number of results */
}

static int l_draw_char (lua_State *L) {
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double c = luaL_checknumber(L, 3);
    draw_char(x,y,c);
    return 0;  /* number of results */
}

static int l_draw_string (lua_State *L) {
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    const char* s = luaL_checkstring(L, 3);
    draw_string(x,y,s);
    return 0;  /* number of results */
}

static int l_draw_start(lua_State *L){
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        //BACKGROUNG//
        SDL_RenderFillRect(game->renderer,NULL);
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        draw_rect(0,0,screen_width,screen_height,0,0,0);
        return 0;
}

static int l_draw_end(lua_State *L){
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(game->renderer, texture, NULL, NULL);
        SDL_RenderPresent(game->renderer);
        return 0;
}

static int l_wait(lua_State *L){
    double ms = luaL_checknumber(L, 1);
    SDL_Delay((uint32_t)ms);
    return 0;
}

static int l_get_event(lua_State *L){
    lua_pushboolean(L,SDL_PollEvent(&game->event));
    return 1;
}

static int l_get_delta_time(lua_State *L){
        lua_pushnumber(L,delta_time);
    return 1;
}
void lua_custom_functions(lua_State *L){
    lua_pushcfunction(L, l_draw_pixel);
    lua_setglobal(L, "draw_pixel");
    lua_pushcfunction(L, l_draw_char);
    lua_setglobal(L, "draw_char");
    lua_pushcfunction(L, l_draw_string);
    lua_setglobal(L, "draw_string");

    lua_pushcfunction(L, l_draw_rect);
    lua_setglobal(L, "draw_rect");

    lua_pushcfunction(L, l_draw_start);
    lua_setglobal(L, "draw_start");
    lua_pushcfunction(L, l_draw_end);
    lua_setglobal(L, "draw_end");

    lua_pushcfunction(L, l_wait);
    lua_setglobal(L, "wait");
    
    lua_pushcfunction(L, l_get_event);
    lua_setglobal(L, "get_event");

    lua_pushcfunction(L, l_get_delta_time);
    lua_setglobal(L, "get_delta_time");

}

#endif
