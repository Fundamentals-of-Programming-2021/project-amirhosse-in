#include <stdio.h>
#pragma once
typedef struct Soldier{
    int x;
    int y;
    int speed_x;
    int speed_y;
    int team;
    int city_id;

    
} Soldier;

typedef struct City{
    int id;
    int x;
    int y;
    int team;//1 means white
    int soldier_counts;
    int soldiers_to_move;
    int growth_rate;
} City;

typedef struct Game{
    int players_count;
    
} Game;


#define map_width 600
#define map_height 600
#define map_cell_side 5

int map[map_height/map_cell_side][map_width/map_cell_side] = {0};

City* cities;
int cities_count = 0;
int cities_available[50];

int players_count = 4;

int primary_anti_soldier_count = 30;
int primary_soldier_count = 40;
int growth_rate_per_second = 3;

int current_mouse_x = 0;
int current_mouse_y = 0;
int is_mouse_pressed = 0;
int pressed_x = 0;
int pressed_y = 0;