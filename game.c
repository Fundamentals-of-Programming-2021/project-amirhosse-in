#ifndef GAME_C
#define GAME_C

#include "game.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "./cfiles/io.c"
#include "./cfiles/soldiers.c"
#include "./cfiles/map.c"
#include "./cfiles/explosion.h"
#include "./cfiles/ai.h"

//protypes
//void draw_camps(SDL_Renderer* renderer);
void attack(int base_id, int dest_id);
//this function initialize primary thing like generating map and etc.
void game_generator(SDL_Renderer* renderer){
    load_user_names();
    save_user_names();
    init_textures(renderer);
    //loading files
    explosion_effect = Mix_LoadWAV("./files/explosion.wav");
    //end of loading files
    clock_t aaaa = clock();
    srand(time(NULL));
    int city_count = rand()%5+40;
    soldiers = (Soldier*) malloc(sizeof(Soldier));
    for(int i=0;i<5;i++){
        ai_tick[i]=rand()%5;
        printf("we rand %d\n", ai_tick[i]);
    }
    explosions = (Explosion*) malloc(sizeof(Soldier));
    //map_generator( &city_count);
    load_map("map1.map");
    remove_border_city();
    cities = find_camps();
    clean_map_from_non_camps_city();
    assign_camps_to_players();
    first_init_soldiers();
    save_map("map1.map");
    save_map_camps("camps1.cmp");
    clock_t bbbb = clock();
    double time_s  = (double)(bbbb-aaaa)/CLOCKS_PER_SEC;
    printf("we could generete %d cities in %lf seconds\n", cities_count, time_s);

}

//this function draw a mouse curser and moving line
void mouse_hover(SDL_Renderer* renderer, int x, int y,int pressed_x, int pressed_y, int is_pressed){
    if(x>= map_start_x && x< map_start_x + map_width && y >= map_start_y && y < map_start_y + map_height){
        int x_on_screen = x, y_on_screen = y;
        x-=map_start_x;
        y-=map_start_y;
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

//when user leave the mouse up, we call this functino to detect attack
void detect_attack(int base_x, int base_y, int dest_x, int dest_y){
    if(dest_x >= map_start_x && dest_y >= map_start_y && base_x >= map_start_x && base_y >= map_start_y
    && dest_x < map_start_x + map_width && dest_y < map_start_y+map_height && base_x < map_start_x + map_width && base_y < map_start_y+map_height){
        base_x -= map_start_x; base_x /= map_cell_side;
        base_y -= map_start_y; base_y /= map_cell_side;
        dest_x -= map_start_x; dest_x /= map_cell_side;
        dest_y -= map_start_y; dest_y /= map_cell_side;
        
        if(map[dest_y][dest_x] * map[base_y][base_x] == 0 || abs(map[dest_y][dest_x]) == abs(map[base_y][base_x])){
            add_new_message("Invalid attack!", red,0);
        }else{
            attack(abs(map[base_y][base_x]), abs(map[dest_y][dest_x]));
        }
    }
    else {
        //invalid attack
    }
}



#endif