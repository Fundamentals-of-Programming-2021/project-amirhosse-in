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
    int x;
    if(id==0) x = -100;
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
        case 1:out=0xff3539E5;break;
        case -1:out=0xff1C1CB7;break;
        case 2:out=0xff9F3F30;break;
        case -2:out=0xff7E231A;break;
        case 3:out=0xff6B7900;break;
        case -3:out=0xff404D00;break;
        case 4:out=0xff7A6E54;break;
        case -4:out=0xff383226;break;
        default: printf("we found a bug here\n");break;
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
        char* buffer = (char*) malloc(sizeof(char) * 20);
        sprintf(buffer, "%d", cities[i].id);
        stringRGBA(renderer, x +2 , y + 3, buffer, 0,0,0,255);
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
            circleColor(renderer, x_on_screen, y_on_screen, 10,map[y][x] == 0? 0 :color_picker(-abs(map[y][x])));
            circleColor(renderer, x_on_screen, y_on_screen, 9,map[y][x] == 0? 0 : color_picker(-abs(map[y][x])));
            circleColor(renderer, x_on_screen, y_on_screen, 8,map[y][x] == 0? 0 :color_picker(-abs(map[y][x])));
            filledCircleColor(renderer, x_on_screen, y_on_screen, 3,map[y][x] == 0? 0 : color_picker(-abs(map[y][x])));
        }
        if(is_pressed){
            SDL_RenderDrawLine(renderer, pressed_x, pressed_y, x_on_screen,y_on_screen);
        }
    }
}

void detect_attack(int base_x, int base_y, int dest_x, int dest_y){
    if(dest_x >= 100 && dest_y >= 100 && base_x >= 100 && base_y >= 100
    && dest_x < 700 && dest_y < 700 && base_x < 700 && base_y < 700){
        base_x -= 100; base_x /= map_cell_side;
        base_y -= 100; base_y /= map_cell_side;
        dest_x -= 100; dest_x /= map_cell_side;
        dest_y -= 100; dest_y /= map_cell_side;
        
        if(map[dest_y][dest_x] * map[base_y][base_x] == 0 || abs(map[dest_y][dest_x]) == abs(map[base_y][base_x])){
            //invalid attack
            printf("it wasn't a valid attack\n");
        }else{
            printf("you attacked from %d to %d\n", map[base_y][base_x],map[dest_y][dest_x]);
        }
    }
    else {
        //invalid attack
    }
}

