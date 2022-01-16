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
//protypes
void draw_camps(SDL_Renderer* renderer);

//this function initialize primary thing like generating map and etc.
void game_generator(){
    srand(time(NULL));
    int city_count = rand()%5+40;

    map_generator( &city_count);
    cities = find_camps();
    clean_map_from_non_camps_city();
    assign_camps_to_players();
    save_map("map1.map");
    printf("we could generete %d cities\n", cities_count);
}
//this function is used for drawing map and border (negative numbers specifies borders) & this function uses id_to_city_index() to determine owner of city
int color_picker(int id){
    int out = 0;
    int x = id_to_city_index(id);
    //printf("%d\n",x);
    switch (x)
    {
        case -100:out = 0;break;
        case 90:out = 0x414C6D;break;
        case -90:out = 0x23273E;break;
        case 1:out=0x3539E5;break;
        case -1:out=0x1C1CB7;break;
        case 2:out=0x9F3F30;break;
        case -2:out=0x7E231A;break;
        case 3:out=0x6B7900;break;
        case -3:out=0x404D00;break;
        case 4:out=0x7A6E54;break;
        case -4:out=0x383226;break;
        default: printf("%d\n",x);break;
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
            boxColor(renderer, x, y, x+map_cell_side, y+map_cell_side,color_picker(map[i][j]));
        }
    }
    draw_camps(renderer);
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
        SDL_SetRenderDrawColor(renderer, 0,255,0,255);
        SDL_RenderFillRect(renderer, &r);

        char* buffer = malloc(sizeof(char) * 50);
        sprintf(buffer, "team: %d, id: %d", cities[i].team, cities[i].id);
        stringRGBA(renderer, x, y-5, buffer, 0, 0, 0, 255);
        free(buffer);
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
            circleColor(renderer, x_on_screen, y_on_screen, 10,map[y][x] == 0? 0 : color_picker(map[y][x]));
            circleColor(renderer, x_on_screen, y_on_screen, 9,map[y][x] == 0? 0 : color_picker(map[y][x]));
            circleColor(renderer, x_on_screen, y_on_screen, 8,map[y][x] == 0? 0 :  color_picker(map[y][x]));
            filledCircleColor(renderer, x_on_screen, y_on_screen, 3,map[y][x] == 0? 0 : color_picker(map[y][x]));
        }
        if(is_pressed){
            SDL_RenderDrawLine(renderer, pressed_x, pressed_y, x_on_screen,y_on_screen);
        }
    }
}

