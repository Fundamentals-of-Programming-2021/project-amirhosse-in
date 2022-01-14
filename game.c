#include "game.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
///consts
#define map_width 600
#define map_height 600
#define map_cell_side 5
//global variables
int map[map_height/map_cell_side][map_width/map_cell_side] = {0};
int city_border[4000][2];
//protypes
int find_big_area(int n);
typedef struct Soldier{
    int x;
    int y;
    int speed;
    int team;
    
} Soldier;

//1 means white
typedef struct City{
    int id;
    int x;
    int y;
    int team;
    int soldier_counts;
    int growth_rate;
} City;

typedef struct Game{
    int players_count;
    
} Game;



void dfs_border(int row, int column, int map_flag[][map_width/map_cell_side], int* border_count, int city_id){
    if(map[row][column] == 0){
        city_border[*border_count][0] = row;
        city_border[*border_count][1] = column;
        *border_count += 1;
        return ;
    }
    for(int i=-1; i<=1;i++){
        for(int j=-1; j<=1; j++){
            if(row + i >= 0 && row + i < map_height/map_cell_side 
            && column +j >= 0 && column + j < map_width/map_cell_side
            && map_flag[row+i][column+j] == 0 
            && (map[row+i][column+j] == 0 || map[row+i][column+j] == city_id) ){
                map_flag[row+i][column+j] = 1;
                dfs_border(row+i, column+j, map_flag, border_count, city_id); 
            }
        }
    }
}

int city_border_finder(int row, int column, int city_id){
    int map_flag[map_height/map_cell_side][map_width/map_cell_side] = {0};
    int border_count = 0;
    dfs_border(row, column, map_flag, &border_count, city_id);
    return border_count;
}


int fill_city(int row, int column,int city_id){
    int city_area = rand()%100 + 250;
    map[row][column] = city_id;
    int border_count = 0;
    //primary fill the city
    
    while(city_area--){
        border_count = city_border_finder(row, column, city_id);
        if(border_count!=0){
            int new_cell = rand()%border_count;
            map[ city_border[new_cell][0] ][ city_border[new_cell][1] ] = city_id;
        }else {
            //handle the small map
        }
    }
    //full fill the city
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            if(map[i][j] == 0){
                int flag = 0;
                for(int p=-1; p<=1;p++){
                    for(int q=-1; q<=1; q++){
                        if(i+p >=0 && i+p < map_height/map_cell_side 
                        && j+q >= 0 && j+q < map_width/map_cell_side
                        && map[i+p][j+q] == city_id) flag ++;
                    }
                }
                if( /*( (i*j == 0 || i==map_height/map_cell_side-1 || j==map_width/map_cell_side-1) && flag>=2 ) ||*/ (i*j != 0 && flag >= 5) ) map[i][j] = city_id;
            }
        }
    }
    return city_border_finder(row, column, city_id);
}

void map_generator(int city_count){
    int n =0;
    for(int i=0;i<city_count;i++){
        //n equals to borders count here
        n = find_big_area(n);
        if(n != -1){
            //n equals to best border point to fill city from
            n = fill_city(city_border[n][0], city_border[n][1], i+1);
        }else {
            
        }
    }
}

void game_generator(){
    srand(time(NULL));
    int players_count = rand()%4+1;
    int city_count = rand()%5+35;
    map_generator(city_count);
}
void dfs_for_calculate_area(int row, int column,int flag[][map_width/map_cell_side], int number, int* count){
    flag[row][column] = 1;
    *count+=1;
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(row+i >=0 && row+i < map_height/map_cell_side
            && column+j >=0 && column+j < map_width/map_cell_side
            && flag[row+i][column+j] == 0 && map[row+i][column+j] == number){
                dfs_for_calculate_area(row+i, column+j, flag , number, count);
            }
        }
    }
}
//find a big area from border city to fill a new city
int find_big_area(int n){
    int out =0;
    for(out ; out < n; out++){
        int flag[map_height/map_cell_side][map_width/map_cell_side] = {0};
        int count = 0;
        dfs_for_calculate_area(city_border[out][0], city_border[out][1], flag, 0, &count);
        if(count>=100) break;
    }
    return out==n?-1:out;
}

int color_picker(int id){
    int out = 0;
    switch(id){
        case 0:out = 0xff646464;break;
        case 1:out = 0xFFCF75F5;break;
        case 2:out = 0xff109238;break;
        case 3:out = 0xff2b2bff;break;
        case 4:out = 0xffff2bc5;break;
        case 5:out = 0xff0000ff;break;
        case 6:out = 0xff00c3ff;break;
        case 7:out = 0xff451858;break;
        case 8:out = 0xff18a221;break;
        case 9:out = 0xff3d2f21;break;
        case 10:out = 0xffa6a595;break;
        case 11:out = 0xff778F14;break;
        case 12:out = 0xff000000;break;
        case 13:out = 0xffff0000;break;
        case 14:out = 0xffff0000;break;
        case 15:out = 0xffff0000;break;
        case 16:out = 0xffff0000;break;
        case 17:out = 0xffff0000;break;
        default :out = 0x00646464;break;
    }
    return out;
}

void draw_map(SDL_Renderer* renderer){
    int height_base = 100;
    int width_base = 100;
    int x,y;
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            x = j*map_cell_side + width_base;
            y = i*map_cell_side + height_base; 
            boxColor(renderer, x, y, x+map_cell_side, y+map_cell_side,map[i][j] == 0? 0 : color_picker(map[i][j]%11+2));
        }
    }
}

