#include "game.h"
#include <time.h>
#define map_width 600
#define map_height 600
#define map_cell_side 10

int map[map_height/map_cell_side][map_width/map_cell_side] = {0};
int city_border[1000][2];

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
            && (map[row+i][row+j] == 0 || map[row+i][row+j] == city_id) ){
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


void fill_city(int row, int column,int city_id){
    printf("we are here1 %d",city_id);
    int city_area = rand()%100 + 200;
    map[row][column] = city_id;
    int border_count = 0;
    //primary fill the city
    
    while(city_area--){
        border_count = city_border_finder(row, column, city_id);
        int new_cell = rand()%border_count;
        map[ city_border[new_cell][0] ][ city_border[new_cell][1] ] = city_id;
    }
    printf("we are here2 %d",city_id);
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
                if( (i*j == 0 && flag>=3 ) || (i*j != 0 && flag >= 5) ) map[i][j] = city_id;
            }
        }
    }
    printf("we are here3 %d\n",city_id);
    city_border_finder(row, column, city_id);
}

void game_generator(){
    srand(time(NULL));
    int players_count = rand()%4+1;
    int city_count = rand()%5+8;
    fill_city(0,0,1);
    
    for(int i=0;i<60;i++){
        for(int j=0;j<60;j++){
            printf("%d", map[i][j]);
        }
        printf("\n");
    }
    fill_city(0, 0, 2);
}




