#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "game.h"
#include "game.c"
//echo we are testing this git repo
const int FPS = 60 ;
const int window_height = 800;
const int window_width = 1000;

/*
 getting keys
 void moveCircle(const Uint8* keys, double* snake_x, double* snake_y) {
    if(keys[SDL_SCANCODE_LEFT]){
        *snake_x -= step;
    } if(keys[SDL_SCANCODE_RIGHT]){
        *snake_x += step;
    } if(keys[SDL_SCANCODE_UP]){
        *snake_y -= step;
    }if(keys[SDL_SCANCODE_DOWN]){
        *snake_y += step;
    }
}*/

const int EXIT = 12345;
int handleEvents(SDL_Renderer* renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return EXIT;
        if (event.type == SDL_KEYDOWN){
            const Uint8* keys = SDL_GetKeyboardState(NULL);
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
                detect_attack(pressed_x, pressed_y, event.button.x, event.button.y);
            }
        }

    }
    return 0;
}

int main() {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //adding test picture
    SDL_Surface* surface = SDL_LoadBMP("./files/coffee.bmp");
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,0xFF,0,0xFF));
    SDL_Texture* coffee = SDL_CreateTextureFromSurface(renderer,surface);
    surface = SDL_LoadBMP("./files/2.bmp");
    SDL_Texture* cloud = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    game_generator();
    SDL_Rect rect = {.x = 10, .y=10, .h = 100, .w=100};
    //end of adding test picture
    int begining_of_time = SDL_GetTicks();
    while (1) {
        start_ticks = SDL_GetTicks();
        if (handleEvents(renderer) == EXIT) break;
    	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    	SDL_RenderClear(renderer);
        draw_map(renderer);
        mouse_hover(renderer, current_mouse_x,current_mouse_y,pressed_x, pressed_y, is_mouse_pressed);
        char* buffer = malloc(sizeof(char) * 50);
        sprintf(buffer, "amnam's score: %d   elapsed time: %dms", start_ticks,start_ticks - begining_of_time);
        stringRGBA(renderer, 5, 5, buffer, 0, 0, 0, 255);
        //SDL_RenderCopyEx(renderer, getImageTexture(renderer,"./files/coffee.bmp"), NULL,&rect, 45, NULL, SDL_FLIP_HORIZONTAL);
         
        free(buffer);
    	SDL_RenderPresent(renderer);
        SDL_Delay ( 1000 / FPS);
    }
    SDL_DestroyTexture(cloud);
    SDL_DestroyTexture(coffee);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
