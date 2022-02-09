#ifndef MAP_C
#define MAP_C

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.c"
#include "soldiers.c"
#include "camps.c"
#pragma once
int city_border[4000][2] ;
void draw_explosions(SDL_Renderer* renderer);
//prototype
int find_big_area( int n);
void dfs_for_calculate_area(int row, int column,int flag[][map_width/map_cell_side], int number, int* count);
void map_generator( int* city_count);
int fill_city( int row, int column,int city_id);
int city_border_finder(int row, int column, int city_id);
void specify_border();
void draw_map(SDL_Renderer* renderer);
int color_picker(int id);
void remove_border_city();
void clean_map_from_non_camps_city();
void dfs_border(int row, int column, int map_flag[][map_width/map_cell_side], int* border_count, int city_id);

//this function specifies a city's border to make it bigger by coloring a new cell
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
                dfs_border( row+i, column+j, map_flag, border_count, city_id); 
            }
        }
    }
}

//this function calls dfs_border() to find a city's border to make it bigger and returns count of borders cells
int city_border_finder(int row, int column, int city_id){
    int map_flag[map_height/map_cell_side][map_width/map_cell_side] = {0};
    int border_count = 0;
    dfs_border( row, column, map_flag, &border_count, city_id);
    return border_count;
}
//this function by calling city_border_finder() find new cells to make city bigger and retrurn final city's border to start to fill a new city
int fill_city(int row, int column,int city_id){
    int city_area = rand()%1000 + 2500;
    map[row][column] = city_id;
    int border_count = 0;
    //primary fill the city
    
    while(city_area--){
        border_count = city_border_finder( row, column, city_id);
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
    return city_border_finder( row, column, city_id);
}

//this function generates a map
void map_generator( int* city_count){
    int n =-1;
    int final_counts = 0;
    for(int i=0;i<*city_count;i++){
        //n equals to borders count here
        n = find_big_area( n);
        if(n != -1){
            //n equals to best border point to fill city from
            n = fill_city(city_border[n][0], city_border[n][1], i+1);
            final_counts++;
        }else {
            for(int j=0;j<map_width/map_cell_side;j++){
                if(map[map_height/map_cell_side-1][j] == 0){
                    n = fill_city( map_height/map_cell_side-1, j, i+1);
                    final_counts++;
                    break;
                }
            }
        }
    }
    *city_count = final_counts;
    specify_border();
    
}

//it just called by find_big_area()
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
int find_big_area( int n){
    int out =0;
    for(int i ; i < n; i++){
        out = i;
        int flag[map_height/map_cell_side][map_width/map_cell_side] = {0};
        int count = 0;
        dfs_for_calculate_area( city_border[i][0], city_border[i][1], flag, 0, &count);
        if(count>=100) break;
    }
    if(n == -1) return 0;
    else return out == n ? -1: out;
}

//this function specifies city borders and make them -1*id in map array (for coloring darker)
void specify_border(){
    for(int i=0; i< map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            if(map[i][j] !=0){
                int flag = 0;
                for(int q=-1;q<=1;q++){
                    for(int p=-1;p<=1;p++){
                        if(i+q >=0 && i+q< map_height/map_cell_side
                        && j+p >=0 && j+p< map_width/map_cell_side
                        && map[i+q][j+p] != map[i][j] && map[i+q][j+p] != -1*map[i][j]) flag = 1;
                        if(i * j == 0 || i+1 == map_height/map_cell_side || j + 1 == map_width / map_cell_side) flag = 1;
                    }
                }
                if(flag == 1) map[i][j] *= -1;
            }
        }
    }
}

//there is some cities with no camps in them and this functino will remove (0) them
void clean_map_from_non_camps_city(){
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_height/map_cell_side;j++){
            int x = map[i][j];
            x = (x<0 ? -x : x);
            if(cities_available[ x ] == 0) map[i][j] = 0;
        }
    }
}

//this function removes a city with it's id
void remove_city_with_id(int id){
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            if(map[i][j] == id || map[i][j] == -id){
                map[i][j] = 0;
            }
        }
    }
}

//this function remove cities wich they are in map borders
void remove_border_city(){
    for(int i=0;i<map_height/map_cell_side;i++){
        remove_city_with_id(map[i][0]);
        remove_city_with_id(map[i][map_width/map_cell_side-1]);
    }
    for(int i=0;i<map_width/map_cell_side;i++){
        remove_city_with_id(map[0][i]);
        remove_city_with_id(map[map_height/map_cell_side-1][i]);
    }
}

//this function is used for drawing map and border (negative numbers specifies borders) & this function uses id_to_city_index() to determine owner of city
int color_picker(int id){
    int out = 0;
    int x;
    if(id==0){ x = -100;}
    else{ 
        x = id_to_city_index(abs(id));
        if(cities[x].team != 0) x = cities[x].team * (id < 0? -1 : 1);
        else x = 90 * (id < 0? -1 : 1);
    }
    switch (x)
    {
        case -100:out = 0xff;break;
        case 90:out = 0xff414C6D;break;
        case -90:out = 0xff23273E;break;
        case 1:out=0xff3539E5;break;//red
        case -1:out=0xff1C1CB7;break;
        case 2:out=0xff9F3F30;break;//blue
        case -2:out=0xff7E231A;break;
        case 3:out=0xff6B7900;break;//green
        case -3:out=0xff404D00;break;
        case 4:out=0xff7A6E54;break;//gray
        case -4:out=0xff383226;break;
        default: printf("we found a bug here%d and %d and index:%d\n", x,id, id_to_city_index(abs(id)));break;
    }
    return out;
}

//this function draws map on renderer
void draw_map(SDL_Renderer* renderer){
    SDL_Rect bg_rect = {.x = 0, .y = 0, .w = window_width, .h = window_height };
    int x,y;
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            x = j*map_cell_side + map_start_x;
            y = i*map_cell_side + map_start_y; 
            boxColor(renderer, x, y, x+map_cell_side, y+map_cell_side,color_picker(map[i][j]));
        }
    }
}

#endif