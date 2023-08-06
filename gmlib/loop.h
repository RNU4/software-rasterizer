#include "gmlib.h"


void game_init(game_t *game){
    texture = NULL;
    texture = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING , screen_width, screen_height);
}


void game_keyboard(game_t *game){

    //Default
    //while (!game->close){
        char *c;
        while (SDL_PollEvent(&game->event)) {
            switch (game->event.type) {

                case SDL_TEXTINPUT:
                    // Add new text onto the end of our text
                    //strcat(text,game->event.text.text);
  
                break;

                case SDL_KEYDOWN:

                    switch (game->event.key.keysym.scancode) { 

                        case SDL_SCANCODE_ESCAPE:
                            // handling of game->close button//
                            game->close = 1;
                        break;
                        case SDL_SCANCODE_RETURN:
                  
                        break;
                        case SDL_SCANCODE_BACKSPACE:
    
                        break;
                        default:break;
                    }
                    

                break;
                
                default:break;
            }
        }
    //}
}

void game_step(game_t *game){
    
}

int8_t test_img[8] = {
  0B01010100,
  0B00101010,
  0B00111100,
  0B00011000,
  0B00011000,
  0B00011000,
  0B00011000,
  0B00111100,
};
/*
void game_draw(game_t *game){
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        //BACKGROUNG//
        SDL_RenderFillRect(game->renderer,NULL);
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        draw_rect(0,0,screen_width,screen_height,255,255,255);
        draw_rect(1,1,screen_width-1,screen_height-1,0,0,0);
                
        draw_string(game,8,8,game->text.string);
        draw_bitmap(40,40,8,8,test_img);
        SDL_UnlockTexture(texture);

        
        SDL_RenderCopy(game->renderer, texture, NULL, NULL);
        
        SDL_RenderPresent(game->renderer);
}