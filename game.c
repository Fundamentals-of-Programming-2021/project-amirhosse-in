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
#include <SDL2/SDL_ttf.h>
#include "./cfiles/io.h"
#include "./cfiles/soldiers.h"
#include "./cfiles/map.h"
#include "./cfiles/explosion.h"
#include "./cfiles/ai.h"
#include "./cfiles/getuser.h"
#include "./cfiles/menu.h"
#include "./cfiles/map_menu.h"
#include "./cfiles/potions.h"
#include "./cfiles/standing.h"

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
    detect_click_camps(dest_x, dest_y);
    if(dest_x >= map_start_x && dest_y >= map_start_y && base_x >= map_start_x && base_y >= map_start_y
    && dest_x < map_start_x + map_width && dest_y < map_start_y+map_height && base_x < map_start_x + map_width && base_y < map_start_y+map_height){
        base_x -= map_start_x; base_x /= map_cell_side;
        base_y -= map_start_y; base_y /= map_cell_side;
        dest_x -= map_start_x; dest_x /= map_cell_side;
        dest_y -= map_start_y; dest_y /= map_cell_side;
        
        if(map[dest_y][dest_x] * map[base_y][base_x] == 0 || abs(map[dest_y][dest_x]) == abs(map[base_y][base_x])){
            add_new_message("Invalid attack!", red,0);
        }else{
            attack(abs(map[base_y][base_x]), abs(map[dest_y][dest_x]), 1);
        }
    }
    else {
        //invalid attack
    }
}
void win(SDL_Renderer* renderer){
    int online_teams[5]={0};
    for(int i=0;i<cities_count;i++){
        if(cities[i].team != 0) online_teams[cities[i].team] = 1;
    }
    for(int i=0;i<soldiers_count;i++){
        if(soldiers[i].dest_x!= -1) online_teams[soldiers[i].team]=1;
    }
    int online_teams_count =0;
    int winner=-1;
    for(int i=0;i<5;i++)
    {
        online_teams_count+=online_teams[i];
        if(online_teams[i])winner=i;
    }
    if(online_teams_count == 1){
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        paused_tick =0;
        if(winner == 1){
            users_scores[user_id] += 30;
        }
        else users_scores[user_id] -= 10;
        save_user_names();
        //winner
        SDL_Rect r;
        SDL_QueryTexture(winning_caption[winner-1],NULL, NULL, &r.w, &r.h);
        r.x = 500-(r.w)/2;
        r.y = 400-(r.h)/2;
        switch (winner){
            case 1:{
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_red.r,my_red.g,my_red.b,100);
            }break;
            case 2:{
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_blue.r,my_blue.g,my_blue.b,100);
            }break;
            case 3:{
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_green.r,my_green.g,my_green.b,100);
            }break;
            case 4:{
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_gray.r,my_gray.g,my_gray.b,100);
            }break;
        }
        for(int j=0;j<cities_count;j++) {cities[j].team =0;free(cities[j].dest_id);free(cities[j].soldiers_to_move);}
        for(int j=0;j<50;j++) cities_available[j] =0 ;
        free(cities);
        cities = find_camps();
        assign_camps_to_players();
        for(int j=0;j<cities_count;j++)if(cities[j].team!=0) cities[j].soldier_counts=0;
        free(soldiers);
        soldiers_count =0;
        max_soldiers_count=0;
        soldiers = (Soldier*) malloc(sizeof(Soldier));
        first_init_soldiers();
        SDL_RenderCopyEx(renderer, winning_caption[winner-1], NULL, &r, 0,NULL,0);
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);
        window_state = 1;
    }
}



#endif