#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.h"
#include "io.h"

//prototypes
void create_explosion(int x, int y);
void destory_explosions();
void explosions_watcher();
void draw_explosions(SDL_Renderer* renderer);
SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path);


//this function creates a explosion on x,y
void create_explosion(int x, int y){
    if(max_explosions_count < 1+explosions_count ){
        max_explosions_count ++;
        explosions = (Explosion*) realloc(explosions, sizeof(Explosion) * max_explosions_count);
    }
    explosions[explosions_count].x = x;
    explosions[explosions_count].y = y;
    explosions[explosions_count].state = 1;
    explosions[explosions_count].last_tick = start_ticks;
    explosions_count++;
}

//this function destories a explosion after t seconds
void destory_explosions(){
    for(int i=0;i<explosions_count-1;i++){
        explosions[i] = explosions[i+1];
    }
    explosions_count--;
}

//this function watchs explosions and change their state and remove them
void explosions_watcher(){
    for(int i=0;i<explosions_count;i++){
        if(start_ticks - explosions[i].last_tick > 50){
            if(explosions[i].state != 8){
                explosions[i].state ++; 
                explosions[i].last_tick = start_ticks;
            }else{
                destory_explosions();
                i--;
            }
        }
    }
}

//this funcion draws explosions
void draw_explosions(SDL_Renderer* renderer){
    explosions_watcher();
    char path[] = "./files/explosions/1.bmp";
    SDL_Rect r = {.x= 0, .y =0, .h= explosion_size, .w = explosion_size};
    for(int i=0; i < explosions_count; i++){
        path[19] = explosions[i].state+'0';
        r.x = explosions[i].x;
        r.y = explosions[i].y;
        SDL_Texture* temp_texture = getImageTexture(renderer,path);
        SDL_RenderCopyEx(renderer, temp_texture, NULL,&r, 0, NULL, SDL_FLIP_HORIZONTAL);
        SDL_DestroyTexture(temp_texture);
    } 
}

#endif