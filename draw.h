#ifndef _DRAW_H
#define _DRAW_H
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <strings.h>
#include "font.h"
#include "gmlib/config.h"
#include "init.h"
/*
void draw_pixel(int x,int y,int r,int g,int b){
    if ( (x<screen_width && x>=0 && y<screen_height && y>=0)){
    base = ((Uint8 *)pixels) + (4 * (y * screen_width + x));
    base[0] = b;
    base[1] = g;
    base[2] = r;
    base[3] = COLOR_MAX;
    }
}*/

#define draw_pixel(x,y,r,g,b)({ \
    if ( (x<screen_width && x>0 && y<screen_height && y>0)){ \
    base = ((Uint8 *)pixels) + (4 * (y * screen_width + x));\
    base[0] = b;\
    base[1] = g;\
    base[2] = r;\
    base[3] = COLOR_MAX;\
    } \
}) 


void draw_line2(int x1,int y1,int x2,int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    for (int i = x1; i<x2; i++){
        int y = y1+dy*(i-x1)/dx;
        draw_pixel(i,y,255,255,255);
    }
}

void draw_line(int x1, int y1, int x2, int y2,int r,int g,int b) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        draw_pixel(x1,y1,r,g,b);
        int err2 = err * 2;

        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    draw_pixel(x1,y1,r,g,b);
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b)
{

    draw_line(x1, y1, x2, y2, r, g, b);
    draw_line(x2, y2, x3, y3, r, g, b);
    draw_line(x3, y3, x1, y1, r, g, b);
}

void fill_triangle3(int x1, int y1, int x2, int y2, int x3, int y3,int r,int g,int b) {

for (int i = 0; i<x3-x1; i++)
    draw_triangle(x1+i, y1+i, x2, y2,x3-i,y3-i, r, g, b);
    //draw_triangle(x1, y1, x2, y2,x3,y3, 255, 0, 0);
}


void fill_triangle2(int x1, int y1, int x2, int y2, int x3, int y3,int r,int g,int b) {
    int min_x = x1, max_x = x1, min_y = y1, max_y = y1;

    if (x2 < min_x) min_x = x2;
    if (x3 < min_x) min_x = x3;
    if (x2 > max_x) max_x = x2;
    if (x3 > max_x) max_x = x3;
    if (y2 < min_y) min_y = y2;
    if (y3 < min_y) min_y = y3;
    if (y2 > max_y) max_y = y2;
    if (y3 > max_y) max_y = y3;

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int w0 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
            int w1 = (x3 - x2) * (y - y2) - (y3 - y2) * (x - x2);
            int w2 = (x1 - x3) * (y - y3) - (y1 - y3) * (x - x3);

            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0))
                draw_pixel(x, y,r,g,b);
        }
    }
}




void batch_fill_triangle(stack_t* stack){

    stack->top = 0;

}
void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b) {
    int min_x, max_x, min_y, max_y;
    int dx1 = x2 - x1, dx2 = x3 - x2, dx3 = x1 - x3;
    int dy1 = y2 - y1, dy2 = y3 - y2, dy3 = y1 - y3;

    min_x = (x1 < x2) ? ((x1 < x3) ? x1 : x3) : ((x2 < x3) ? x2 : x3);
    max_x = (x1 > x2) ? ((x1 > x3) ? x1 : x3) : ((x2 > x3) ? x2 : x3);
    min_y = (y1 < y2) ? ((y1 < y3) ? y1 : y3) : ((y2 < y3) ? y2 : y3);
    max_y = (y1 > y2) ? ((y1 > y3) ? y1 : y3) : ((y2 > y3) ? y2 : y3);

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int w0 = dx1 * (y - y1) - dy1 * (x - x1);
            int w1 = dx2 * (y - y2) - dy2 * (x - x2);
            int w2 = dx3 * (y - y3) - dy3 * (x - x3);

            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0))
                draw_pixel(x, y, r, g, b);
        }
    }
}

void draw_rect(int x,int y,int x2,int y2,int r, int g, int b){
for (int i = x+1; i<=x2; i++)
    for (int j = y+1; j<=y2; j++)
        draw_pixel(i,j,r,g,b);

}


void draw_bitmap(int x,int y,int w,int h,int8_t* img){

    for (int i = 0; i<w; i++)
    for (int j = h-1; j>=0; j--){
        if (img[i] & (1<<((h-1)-j))){
            draw_pixel(x+j,y+i,255,255,255);
        }else{
            draw_pixel(x+j,y+i,0,0,0);
        }
    }
}

void draw_char(int x,int y,char c){
    x+=4;
    y+=1;
    for (int i = 0; i<6; i++){
        char c2=getFontLine(c,i);
        
        for (int j = 0; j<5; j++){
            if (c2 & (1<<j)){
                
                draw_pixel(MAX(MIN(x-j,screen_width-1),0),MAX(MIN(y+i,screen_height-1),0),255,255,255);
            }else{
                draw_pixel(MAX(MIN(x-j,screen_width-1),0),MAX(MIN(y+i,screen_height-1),0),0,0,0);
                
            }
        }
        
    }
}


void draw_string(int x,int y,const char* s){
    int len=strlen(s);
    int xx=x;
    for (int i = 0; i<len; i++){
        
        if (s[i]=='\n'){
                y+=8;xx=x;
            }else{
            draw_char(xx,y,s[i]);
            xx+=4;
        }
        
    }
}


#define draw_start()({ \
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);\
        SDL_RenderClear(game->renderer);\
        SDL_RenderFillRect(game->renderer,NULL); \
        SDL_LockTexture(texture, NULL, &pixels, &pitch);\
        draw_rect(0,0,screen_width,screen_height,0,0,0);\
})

#define draw_end()({ \
        SDL_UnlockTexture(texture);\
        SDL_RenderCopy(game->renderer, texture, NULL, NULL);\
        SDL_RenderPresent(game->renderer);\
})

#endif