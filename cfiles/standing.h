#ifndef STANDING_C
#define STANDING_C

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "io.h"

SDL_Texture **standing_names;
SDL_Texture **standing_scores;
SDL_Texture **back_button_standing;
int users_to_show;
void init_textures_standing(SDL_Renderer* renderer){
    back_button_standing = malloc(sizeof(SDL_Texture*) * 2);
    back_button_standing[0] = getTextTexture(renderer, "./files/fonts/liber.ttf", "Back", black);
    back_button_standing[1] = getTextTexture(renderer, "./files/fonts/liber.ttf", "Back", blue);
    sort_scores();
    users_to_show = user_counts > 10 ? 10 : user_counts;
    standing_names = malloc(sizeof(SDL_Texture*) * (users_to_show + 1));
    standing_scores = malloc(sizeof(SDL_Texture*) * (users_to_show + 1));
    for(int i=0;i<users_to_show;i++){
        standing_names[i] = getTextTexture(renderer, "./files/fonts/liber.ttf", user_names[i],i%2?black:blue);
        char score_temp[20] = "";
        sprintf(score_temp, "%d", users_scores[i]);
        standing_scores[i] = getTextTexture(renderer, "./files/fonts/liber.ttf", score_temp,i%2?black:blue);
    }
    standing_names[users_to_show] = getTextTexture(renderer, "./files/fonts/liber.ttf", "Name",black);
    standing_scores[users_to_show] = getTextTexture(renderer, "./files/fonts/liber.ttf", "Score",black);
}
void destory_textures_standing(){
    for(int i=0;i<users_to_show+1;i++){
        SDL_DestroyTexture(standing_names[i]);
        SDL_DestroyTexture(standing_scores[i]);
    }
    SDL_DestroyTexture(back_button_standing[0]);
    SDL_DestroyTexture(back_button_standing[1]);
    free(back_button_standing);
    free(standing_names);
    free(standing_scores);
}

void draw_button_standing(SDL_Renderer* renderer){
    SDL_Rect r;
    //back button
    r.x = 760; r.y = 365;
    r.w = 175; r.h = 70;
    SDL_RenderCopyEx(renderer, btn_main, NULL, &r, 0, NULL,0);
    if(current_mouse_x - r.x <= r.w && current_mouse_x - r.x >=0 
    && current_mouse_y - r.y <= r.h && current_mouse_y - r.y >=0){
        SDL_QueryTexture(back_button_standing[1], NULL, NULL, &r.w, &r.h);
        r.x += (175-r.w)/2;
        r.y += (70-r.h)/2;
        SDL_RenderCopyEx(renderer, back_button_standing[1], NULL, &r, 0,NULL, 0);
    }
    else{
        SDL_QueryTexture(back_button_standing[0], NULL, NULL, &r.w, &r.h);
        r.x += (175-r.w)/2;
        r.y += (70-r.h)/2;
        SDL_RenderCopyEx(renderer, back_button_standing[0], NULL, &r, 0,NULL, 0);
    }
    
}

void standing_renderer(SDL_Renderer* renderer){
    SDL_Rect r = {.x = 0, .y = 0, .h = window_height, .w = window_width};
    SDL_RenderCopyEx(renderer, bg_menu, NULL, &r, 0,NULL,0);
    roundedBoxRGBA(renderer, 300,50, 700, 750, 10,255,255,255,100);
    draw_button_standing(renderer);
    //"name"
    SDL_QueryTexture(standing_names[users_to_show], NULL, NULL, &r.w, &r.h);
    r.x = 450 - (r.w)/2;
    r.y = 70;
    SDL_RenderCopyEx(renderer, standing_names[users_to_show], NULL, &r, 0 , NULL, 0); 
    //"score"
    SDL_QueryTexture(standing_scores[users_to_show], NULL, NULL, &r.w, &r.h);
    r.x = 650 - (r.w)/2;
    r.y = 70;
    SDL_RenderCopyEx(renderer, standing_scores[users_to_show], NULL, &r, 0 , NULL, 0); 
    //standing list
    for(int i=0;i<users_to_show;i++){
        SDL_QueryTexture(standing_names[i], NULL, NULL, &r.w, &r.h);
        r.x = 450 - (r.w)/2;
        r.y = 130 + 60 * i;
        SDL_RenderCopyEx(renderer, standing_names[i], NULL, &r, 0 , NULL, 0); 
        SDL_QueryTexture(standing_scores[i], NULL, NULL, &r.w, &r.h);
        r.x = 650 - (r.w)/2;
        SDL_RenderCopyEx(renderer, standing_scores[i], NULL, &r, 0 , NULL, 0); 
    }
}
void detect_click_standing(int x, int y){
    // r.x = 760; r.y = 365;
    // r.w = 175; r.h = 70;
    if(x >= 760 && x <= 935 && y >= 365 && y <= 435){
        destory_textures_standing();
        window_state =1;
    }   
}
#endif