#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "game.h"
#include "game.c"
#include "./cfiles/menu.c"
//echo we are testing this git repo
const int FPS = 30 ;
const int EXIT = 12345;
int handleEvents(SDL_Renderer* renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return EXIT;
        if (event.type == SDL_KEYDOWN){
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            if(keys[SDL_SCANCODE_RIGHT]){
                 //Mix_PlayChannel(-1, jumpEffect, 0);
            }
        }
        if( event.type == SDL_MOUSEMOTION){
                //event.motion.x and event.motion.y 
                current_mouse_x = event.motion.x;
                current_mouse_y = event.motion.y;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(event.button.button == SDL_BUTTON_LEFT){
                is_mouse_pressed = 1;
                pressed_x = event.button.x;
                pressed_y = event.button.y;
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP){
            if(event.button.button == SDL_BUTTON_LEFT){
                is_mouse_pressed = 0;
                if(window_state == 2){
                    detect_attack(pressed_x, pressed_y, event.button.x, event.button.y);
                }else if(window_state == 1){
                    detect_click(event.button.x, event.button.y);
                }
            }
        }
    }
    return 0;
}

int main() {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	SDL_Window* window = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    game_generator(renderer);
    init_buttons();
    int begining_of_time = SDL_GetTicks();
    while (1) {
        start_ticks = SDL_GetTicks();
        if (handleEvents(renderer) == EXIT) break;
    	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    	SDL_RenderClear(renderer);
        if(window_state == 1){
            draw_buttons(renderer);
        }else if(window_state == 2){
            draw_map(renderer);
            draw_camps(renderer);
            draw_soldiers(renderer);
            soldier_watcher();
            city_watcher();
            draw_explosions(renderer);
            ai();
            mouse_hover(renderer, current_mouse_x,current_mouse_y,pressed_x, pressed_y, is_mouse_pressed);
            char* buffer = malloc(sizeof(char) * 50);
            sprintf(buffer, "amnam's score: %d   elapsed time: %dms", start_ticks,start_ticks - begining_of_time);
            stringRGBA(renderer, 5, 5, buffer, 0, 0, 0, 255);
            free(buffer);   
        }
    	SDL_RenderPresent(renderer);
        SDL_Delay ( 1000 / FPS );
    }
    Mix_FreeChunk(explosion_effect);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
