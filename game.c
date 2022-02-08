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
#include "./cfiles/io.c"
#include "./cfiles/soldiers.c"
#include "./cfiles/map.c"
#include "./cfiles/explosion.h"
#include "./cfiles/ai.h"

//protypes

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
        //winner
        SDL_Rect r;
        SDL_QueryTexture(winning_caption[winner-1],NULL, NULL, &r.w, &r.h);
        r.x = 500-(r.w)/2;
        r.y = 400-(r.h)/2;
        switch (winner){
            case 1:{
                printf("red won");
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_red.r,my_red.g,my_red.b,100);
            }break;
            case 2:{
                printf("blue won");
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_blue.r,my_blue.g,my_blue.b,100);
            }break;
            case 3:{
                printf("green won");
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_green.r,my_green.g,my_green.b,100);
            }break;
            case 4:{
                printf("gray won");
                roundedBoxRGBA(renderer, 350,325,650,475,10,my_gray.r,my_gray.g,my_gray.b,100);
            }break;
        }
        SDL_RenderCopyEx(renderer, winning_caption[winner-1], NULL, &r, 0,NULL,0);
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);
        window_state = 1;
    }
}



#endif