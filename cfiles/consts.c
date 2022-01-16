#include <stdio.h>
#pragma once
typedef struct Soldier{
    int x;
    int y;
    int speed;
    int team;
    
} Soldier;

typedef struct City{
    int id;
    int x;
    int y;
    int team;//1 means white
    int soldier_counts;
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