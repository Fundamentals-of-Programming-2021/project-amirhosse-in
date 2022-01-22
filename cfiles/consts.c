#include <stdio.h>
#pragma once
typedef struct Soldier{
    int x;
    int y;
    double speed_x;
    double speed_y;
    int dest_x;
    int dest_y;//equals -1 if be in destination
    int team;
    int city_id;//equals to map[i][j]
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
} City;

typedef struct Game{
    int players_count;
    
} Game;


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

Soldier* soldiers;
int soldiers_count = 0;
int primary_anti_soldier_count = 30;
int primary_soldier_count = 40;
int growth_rate_per_second = 3;
double soldier_speed = 0.003;
int minimum_length_for_collision = 5;
int soldiers_size = 5;

int current_mouse_x = 0;
int current_mouse_y = 0;
int is_mouse_pressed = 0;
int pressed_x = 0;
int pressed_y = 0;