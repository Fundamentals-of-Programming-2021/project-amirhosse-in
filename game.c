#include "game.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "./cfiles/io.c"
#include "./cfiles/objects.c"
#include "./cfiles/map.c"
///consts
#define map_width 600
#define map_height 600
#define map_cell_side 5

//protypes
void draw_camps(SDL_Renderer* renderer);


//global variables
int map[map_height/map_cell_side][map_width/map_cell_side] = {0};

City* cities;
int cities_count = 0;
int cities_available[50];// cities_available[city_id] = 1

//this function initialize primary thing like generating map and etc.
void game_generator(){
    srand(time(NULL));
    int players_count = rand()%4+1;
    int city_count = rand()%5+40;

    map_generator(map, &city_count);
    find_camps(map , cities_available,&cities_count,  cities);

    save_map(map_height/map_cell_side, map_width/map_cell_side, map , "map1.map");
    printf("we could generete %d cities\n", cities_count);
}
//this function is used for drawing map and border (negative numbers specifies borders)
int color_picker(int id){
    int out = 0;
    switch(id){
        case 0:out = 0xff646464;break;
        case  1: out = 0xFF2828C6;break;
        case -1: out = 0xFF1C1CB6;break;
        case  2: out = 0xff8C144A;break;
        case 3:out = 0xff933528;break;
        case -3: out= 0xff7E231A;break;
        case 4:out = 0xffBD7702;break;
        case-4:out = 0xff9B5701;break;
        case 5:out = 0xff5C6900;break;
        case-5:out = 0xff404D00;break;
        case 6:out = 0xff2F8B55;break;
        case -6:out = 0xff1E6933;break;
        case 7:out = 0xff25A8F9;break;
        case-7:out = 0xff177FF5;break;
        case 8:out = 0xff4F4737;break;
        case -8:out= 0xff383226;break;
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

//this function draws map on renderer
void draw_map(SDL_Renderer* renderer){
    int height_base = 100;
    int width_base = 100;
    int x,y;
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            x = j*map_cell_side + width_base;
            y = i*map_cell_side + height_base; 
            boxColor(renderer, x, y, x+map_cell_side, y+map_cell_side,map[i][j] == 0? 0 : color_picker(map[i][j]%10));
        }
    }
    //draw_camps(renderer);
}
//this function will be called by draw_map() and draws camps
void draw_camps(SDL_Renderer* renderer){
    for(int i=0;i<cities_count;i++){
        int x = cities[i].x * map_cell_side + 100;
        int y = cities[i].y * map_cell_side + 100;
        SDL_Rect r;
        r.x = x; 
        r.y = y;
        r.w = 25;
        r.h = 25;
        SDL_RenderFillRect(renderer, &r);
    }
}
//this function draw a mouse curser and moving line
void mouse_hover(SDL_Renderer* renderer, int x, int y,int pressed_x, int pressed_y, int is_pressed){
    if(x>=100 && x<700 && y>=100 && y<700){
        int x_on_screen = x, y_on_screen = y;
        x-=100;
        y-=100;
        x/=map_cell_side;
        y/=map_cell_side;
        if(map[y][x] != 0){
            circleColor(renderer, x_on_screen, y_on_screen, 10,map[y][x] == 0? 0 : color_picker(-abs(map[y][x]%10)));
            circleColor(renderer, x_on_screen, y_on_screen, 9,map[y][x] == 0? 0 : color_picker(-abs(map[y][x]%10)));
            circleColor(renderer, x_on_screen, y_on_screen, 8,map[y][x] == 0? 0 : color_picker(-abs(map[y][x]%10)));
            filledCircleColor(renderer, x_on_screen, y_on_screen, 3,map[y][x] == 0? 0 : color_picker(-abs(map[y][x]%10)));
        }
        if(is_pressed){
            SDL_RenderDrawLine(renderer, pressed_x, pressed_y, x_on_screen,y_on_screen);
        }
    }
}



