#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define map_width 600
#define map_height 600
#define map_cell_side 5

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


int check_is_valid_camp(int map[map_height/map_cell_side][map_width/map_cell_side], int row, int column){
    int base_color = map[row][column];
    int flag = 1;
    for(int i=-4;i <7;i++){
        for(int j=-4; j< 7;j++){
            if(row+i < map_height/map_cell_side && column+j < map_width/map_cell_side
            && row+i >=0 && column+j >=0){
                if(map[row+i][column+j] != base_color) flag =0;
            }
        }
    }
    return flag;
}



void find_camps(int map[][map_width/map_cell_side], int cities_available[50],int* cities_count, City* cities){
    cities = (City*) malloc(sizeof(City));
    for(int i=0;i< map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side; j++){
            if(map[i][j] > 0 && !cities_available[map[i][j]] && check_is_valid_camp(map, i,j)){
                cities_available[map[i][j]] = 1;
                *cities_count += 1;
                if(*cities_count>1)
                cities = (City*) realloc(cities, sizeof(City)* (*cities_count));
                cities[(*cities_count)-1].id = map[i][j];
                cities[(*cities_count)-1].y = i;
                cities[(*cities_count)-1].x = j;
            }
        }
    }
}