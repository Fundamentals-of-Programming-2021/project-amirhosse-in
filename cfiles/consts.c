#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#pragma once
typedef struct Soldier{
    double x;
    double y;
    double speed_x;
    double speed_y;
    double dest_x;
    double dest_y;//equals -1 if be in destination
    int team;
    int city_id;//equals to map[i][j]
    double angle;
} Soldier;

typedef struct City{
    int id;
    int x;
    int y;
    int team;
    int soldier_counts;
    int soldiers_to_move;
    int dest_id;//equals to index of cities array
    int growth_rate;
    int last_tick_for_attack;
    int last_tick_for_growth;
} City;

typedef struct Explosion{
    int x;
    int y;
    int state;
    int last_tick;
} Explosion;



#define map_width 700   
#define map_height 700
#define map_cell_side 2
#define map_start_x 10
#define map_start_y 10


int map[map_height/map_cell_side][map_width/map_cell_side] = {0};

City* cities;
int cities_count = 0;
int cities_available[50];

int players_count = 4;
int player_id = 1;

Soldier* soldiers;
int soldiers_count = 0;
int max_soldiers_count = 0;
int primary_anti_soldier_count = 30;
int primary_soldier_count = 30;
int growth_rate_per_second = 1;
double soldier_speed = 6;
int minimum_length_for_collision = 12;
int max_grow_soldier = 60;
int soldiers_size = 30;
int parallel_line_distance = 7; 


int current_mouse_x = 0;
int current_mouse_y = 0;
int is_mouse_pressed = 0;
int pressed_x = 0;
int pressed_y = 0;

int start_ticks;

double explosion_time = 1;
Explosion* explosions;
int explosions_count = 0;
int max_explosions_count = 0;
int explosion_size = 20;

double abs_double(double a){
    return a<0?-a:a;
}
int sign(double a){
    if(a==0) return 0;
    if(a< 0) return -1;
    if(a>0) return 1;
    return -2;
}

int ai_tick[5];

//this function recives non zero abs(map[i][j]) and return index of city 
int id_to_city_index(int map_number){
    for(int i=0;i <cities_count;i++){
        if(cities[i].id == map_number ){
            return i;
        }
    }
    return -1;
}


//audio files
Mix_Chunk *explosion_effect;