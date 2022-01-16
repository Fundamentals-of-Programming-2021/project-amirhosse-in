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

int city_border[4000][2];

//prototype
int find_big_area(int map[map_height/map_cell_side][map_width/map_cell_side], int n);
void dfs_for_calculate_area(int map[map_height/map_cell_side][map_width/map_cell_side], int row, int column,int flag[][map_width/map_cell_side], int number, int* count);
void map_generator(int map[map_height/map_cell_side][map_width/map_cell_side], int* city_count);
int fill_city(int map[map_height/map_cell_side][map_width/map_cell_side], int row, int column,int city_id);
int city_border_finder(int map[map_height/map_cell_side][map_width/map_cell_side],int row, int column, int city_id);
void specify_border(int map[map_height/map_cell_side][map_width/map_cell_side]);

//this function specifies a city's border to make it bigger by coloring a new cell
void dfs_border(int map[map_height/map_cell_side][map_width/map_cell_side],int row, int column, int map_flag[][map_width/map_cell_side], int* border_count, int city_id){
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
                dfs_border(map, row+i, column+j, map_flag, border_count, city_id); 
            }
        }
    }
}

//this function calls dfs_border() to find a city's border to make it bigger and returns count of borders cells
int city_border_finder(int map[map_height/map_cell_side][map_width/map_cell_side],int row, int column, int city_id){
    int map_flag[map_height/map_cell_side][map_width/map_cell_side] = {0};
    int border_count = 0;
    dfs_border(map, row, column, map_flag, &border_count, city_id);
    return border_count;
}
//this function by calling city_border_finder() find new cells to make city bigger and retrurn final city's border to start to fill a new city
int fill_city(int map[map_height/map_cell_side][map_width/map_cell_side], int row, int column,int city_id){
    int city_area = rand()%100 + 250;
    map[row][column] = city_id;
    int border_count = 0;
    //primary fill the city
    
    while(city_area--){
        border_count = city_border_finder(map, row, column, city_id);
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
    return city_border_finder(map, row, column, city_id);
}

//this function generates a map
void map_generator(int map[map_height/map_cell_side][map_width/map_cell_side], int* city_count){
    int n =-1;
    int final_counts = 0;
    for(int i=0;i<*city_count;i++){
        //n equals to borders count here
        n = find_big_area(map, n);
        if(n != -1){
            //n equals to best border point to fill city from
            n = fill_city(map, city_border[n][0], city_border[n][1], i+1);
            final_counts++;
        }else {
            for(int j=0;j<map_width/map_cell_side;j++){
                if(map[map_height/map_cell_side-1][j] == 0){
                    n = fill_city(map, map_height/map_cell_side-1, j, i+1);
                    final_counts++;
                    break;
                }
            }
        }
    }
    *city_count = final_counts;
    specify_border(map);
    
}

//it just called by find_big_area()
void dfs_for_calculate_area(int map[map_height/map_cell_side][map_width/map_cell_side], int row, int column,int flag[][map_width/map_cell_side], int number, int* count){
    flag[row][column] = 1;
    *count+=1;
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(row+i >=0 && row+i < map_height/map_cell_side
            && column+j >=0 && column+j < map_width/map_cell_side
            && flag[row+i][column+j] == 0 && map[row+i][column+j] == number){
                dfs_for_calculate_area(map, row+i, column+j, flag , number, count);
            }
        }
    }
}
//find a big area from border city to fill a new city
int find_big_area(int map[map_height/map_cell_side][map_width/map_cell_side], int n){
    int out =0;
    for(out ; out < n; out++){
        int flag[map_height/map_cell_side][map_width/map_cell_side] = {0};
        int count = 0;
        dfs_for_calculate_area(map, city_border[out][0], city_border[out][1], flag, 0, &count);
        if(count>=100) break;
    }
    if(n == -1) return 0;
    else return out == n ? -1: out;
}

//this function specifies city borders and make them -1*id in map array (for coloring darker)
void specify_border(int map[map_height/map_cell_side][map_width/map_cell_side]){
    for(int i=0; i< map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            if(map[i][j] !=0){
                int flag = 0;
                for(int q=-1;q<=1;q++){
                    for(int p=-1;p<=1;p++){
                        if(i+q >=0 && i+q< map_height/map_cell_side
                        && j+p >=0 && j+p< map_width/map_cell_side
                        && map[i+q][j+p] != map[i][j] && map[i+q][j+p] != -1*map[i][j]) flag = 1;
                    }
                }
                if(flag == 1) map[i][j] *= -1;
            }
        }
    }
}

