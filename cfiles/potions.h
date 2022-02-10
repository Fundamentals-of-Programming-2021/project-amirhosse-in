#ifndef POTIONS_C
#define POTIONS_C
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.h"

void create_potion(){
    if(potions_count == 0){
        potions = (Potion*) malloc(sizeof(Potion));
         max_potions_count = 1;
    }else if(max_potions_count < potions_count + 1){
        max_potions_count = potions_count + 1;
        potions = realloc(potions, sizeof(Potion) * (potions_count+1));
    }
    potions_count++;
    //choosing 2 city
    int city1 = rand()%cities_count;
    int city2 = city1;
    while(city2 == city1) city2 = rand()%cities_count;
    double ratio = (double)(rand()%100)/100;
    potions[potions_count-1].x = (double)(cities[city1].x) * ratio + (double)(cities[city2].x) * (1-ratio);
    potions[potions_count-1].y = (double)(cities[city1].y) * ratio + (double)(cities[city2].y) * (1-ratio);
    potions[potions_count-1].x *= map_cell_side; potions[potions_count-1].x += map_start_x;
    potions[potions_count-1].y *= map_cell_side; potions[potions_count-1].y += map_start_y;
    potions[potions_count-1].is_used = 0;
    potions[potions_count-1].team = -1;
    potions[potions_count-1].type = rand()%6;
    potions[potions_count-1].time = start_ticks;
}
void potion_collision(int soldier_index, int potion_index){
    if(abs_double(soldiers[soldier_index].x - potions[potion_index].x) < 16
    && abs_double(soldiers[soldier_index].y - potions[potion_index].y) < 16){
        potion_state[soldiers[soldier_index].team] = potions[potion_index].type + 1;
        potions[potion_index].is_used = 1;
        potions[potion_index].team = soldiers[soldier_index].team;
        potions[potion_index].time = start_ticks;
    }
}
void potion_watcher(){
    for(int i=0;i<soldiers_count;i++){
        for(int j=0;j<potions_count;j++){
            if(soldiers[i].dest_x != -1 && potion_state[soldiers[i].team] == 0 && potions[j].is_used == 0){
               potion_collision(i,j);
            }
        }
    }
    for(int i=0;i<potions_count;i++){
        if(potions[i].is_used == 0 && start_ticks - potions[i].time > 6000){
            for(int j=i;j<potions_count-1;j++){
                potions[i] = potions[i+1];
            }
            potions_count--;
            i--;
        }else if(potions[i].is_used == 1 && start_ticks - potions[i].time > potion_impact_time){
            potion_state[potions[i].team] =0 ;
            for(int j=i;j<potions_count-1;j++){
                potions[i] = potions[i+1];
            }
            potions_count--;
            i--;
        }
    }
    if(start_ticks > next_potion_tick * 1000){
        create_potion();
        next_potion_tick = (start_ticks/1000) + (rand()%4)+3;
    }
}
void draw_potions(SDL_Renderer* renderer){
    SDL_Rect r;
    r.w = 30;
    r.h = 30;
    for(int i=0;i<potions_count;i++){
        if(potions[i].is_used == 0){
            r.x = potions[i].x;
            r.y = potions[i].y;
            SDL_RenderCopyEx(renderer, potions_texture[potions[i].type], NULL, &r, 0,NULL, 0);
        }else{
            for(int j=0;j<5;j++){
                if(potions[i].team == teams_standing[j]){
                    SDL_Rect r;
                    r.x = 900;
                    r.y = 90+80*j;
                    r.h = r.w = 50;
                    SDL_RenderCopyEx(renderer, potions_texture[potions[i].type],NULL, &r,0,NULL, SDL_FLIP_NONE);
                    int temp_color;
                    switch(potions[i].team){
                        case 1: temp_color=0xff3539E5;break;
                        case 2: temp_color=0xff9F3F30;break;
                        case 3: temp_color=0xff6B7900;break;
                        case 4: temp_color=0xff7A6E54;break;
                    }
                    r.x-=30;
                    r.y+=50;
                    r.h=10;
                    r.w =120 - ( ( ((double)(start_ticks)) - potions[i].time)/potion_impact_time) * 120;
                    roundedBoxColor(renderer,r.x, r.y,r.x+r.w,r.y+r.h,5,temp_color);
                }
            }
        }
    }
}

#endif