#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "consts.c"


int check_is_valid_camp(int row, int column){
    int base_color = map[row][column];
    int flag = 1;
    for(int i=-2;i <7;i++){
        for(int j=-2; j< 7;j++){
            if(row+i < map_height/map_cell_side && column+j < map_width/map_cell_side
            && row+i >=0 && column+j >=0){
                if(map[row+i][column+j] != base_color) flag =0;
            }else flag  = 0;
        }
    }
    return flag;
}



City* find_camps(){

    City* cities;
    cities = (City*) malloc(sizeof(City));
    for(int i=0;i< map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side; j++){
            if(map[i][j] > 0 && !cities_available[map[i][j]] && check_is_valid_camp(i,j)){
                cities_available[map[i][j]] = 1;
                cities_count += 1;
                if(cities_count>1)
                cities = (City*) realloc(cities, sizeof(City)* (cities_count));
                cities[(cities_count)-1].id = map[i][j];
                cities[(cities_count)-1].y = i;
                cities[(cities_count)-1].x = j;
                cities[(cities_count)-1].soldier_counts  = 0;
                cities[(cities_count)-1].team = 0;
            }
        }
    }
    return cities;
}

//this function assign [players_count] city to [players_count] player
void assign_camps_to_players(){
    int x = players_count+1;
    while(x--){
        int a = rand()%cities_count;
        if(cities[a].team == 0){
            cities[a].team = x;
        }else x++;
    }
}
//this function recives map[i][j] and return -100 if map[i][j]=0 and returns 90(or -90 for border) if that city doesn't belong to any player else return player_id
int id_to_city_index(int map_number){
    int flag =1;
    if(map_number == 0) return -100;
    if(map_number < 0){
        map_number*=-1;
        flag = -1;
    }
    for(int i=0;i <cities_count;i++){
        if(cities[i].id == map_number ){
            if(cities[i].team == 0) return flag*90;  
            return flag*cities[i].team;
        }
    }
}