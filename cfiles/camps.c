#ifndef CAMPS_C
#define CAMPS_C
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "io.c"
#include "global.c" 
#include "soldiers.c"
//temporary prototypes
void send_soldier(int base_city_index, int dest_city_index, int index);
void generate_soldier(int count, int team, int city_id);
void draw_standing(SDL_Renderer* renderer);
SDL_Texture *getTextTexture(SDL_Renderer *sdlRenderer, char* font_path, char* caption, SDL_Color color);
void draw_camps(SDL_Renderer* renderer);
void city_watcher();
int check_is_valid_camp(int row, int column);
City* find_camps();
void assign_camps_to_players();
int team_number_to_city_index(int team);
int id_to_city_index(int map_number);

//this function is used by find_camps() to check a cell is appropriate for a camp or not
int check_is_valid_camp(int row, int column){
    int base_color = map[row][column];
    int flag = 1;
    for(int i=-5;i < 25;i++){
        for(int j=-5; j< 25;j++){
            if(row+i < map_height/map_cell_side && column+j < map_width/map_cell_side
            && row+i >=0 && column+j >=0){
                if(map[row+i][column+j] != base_color) flag =0;
            }else flag  = 0;
        }
    }
    return flag;
}

//this functions searches for big cities wich can have a camp
City* find_camps(){
    cities_count =0;
    City* cities;
    cities = (City*) malloc(sizeof(City));
    for(int i=0;i< map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side; j++){
            if(map[i][j] > 0 && !cities_available[map[i][j]] && check_is_valid_camp(i,j)){
                cities_available[map[i][j]] = 1;
                cities_count += 1;
                if(cities_count>1)
                cities = (City*) realloc(cities, sizeof(City)* (cities_count));
                cities[(cities_count)-1].id = map[i][j];
                cities[(cities_count)-1].y = i;
                cities[(cities_count)-1].x = j;
                cities[(cities_count)-1].soldier_counts  = primary_anti_soldier_count;
                cities[(cities_count)-1].team = 0;
                cities[(cities_count)-1].soldiers_to_move = (int*) malloc(sizeof(int));
                cities[(cities_count)-1].growth_rate = growth_rate_per_second;
                cities[(cities_count)-1].dest_id = (int*) malloc(sizeof(int));
                cities[(cities_count)-1].last_tick_for_attack = 0;
                cities[(cities_count)-1].last_tick_for_growth = 0;
                cities[(cities_count)-1].max_dest_counts = 0;
                cities[(cities_count)-1].dest_counts= 0;
            }
        }
    }
    return cities;
}

//this function assign [players_count] city to [players_count] player
void assign_camps_to_players(){
    int x = players_count+1;
    while(x-- > 1){
        int a = rand()%cities_count;
        if(cities[a].team == 0){
            cities[a].team = x;
            cities[a].soldier_counts = 0;
        }else x++;
    }
}

//this function return city index from city team
int team_number_to_city_index(int team){
    for(int i=0;i<cities_count;i++){
        if(cities[i].team == team) return i;
    }
    return -1;
}

//this function recives non zero abs(map[i][j]) and return index of city 
int id_to_city_index(int map_number){
    for(int i=0;i <cities_count;i++){
        if(cities[i].id == map_number ){
            return i;
        }
    }
    return -1;
}

//this function sends soldiers for attacking and increase count of soldiers
void city_watcher(){
    for(int i=0;i<cities_count;i++){
        int total_soldier_to_move = 0;
        for(int j=0;j<cities[i].dest_counts;j++){
            total_soldier_to_move += cities[i].soldiers_to_move[j];
        }
        if( total_soldier_to_move > cities[i].soldier_counts){
            total_soldier_to_move -= cities[i].soldier_counts;
            for(int j=cities[i].dest_counts-1; j>=0; j--){
                if(total_soldier_to_move >= cities[i].soldiers_to_move[j]){
                    total_soldier_to_move -= cities[i].soldiers_to_move[j];
                    cities[i].soldiers_to_move[j] = 0;
                    cities[i].dest_counts--;
                }else{
                    cities[i].soldiers_to_move[j] -= total_soldier_to_move;
                    total_soldier_to_move = 0;
                }
            }
        }
        if(cities[i].dest_counts > 0 && start_ticks - cities[i].last_tick_for_attack > 300){
            cities[i].last_tick_for_attack = start_ticks;
            for(int j=0;j<cities[i].dest_counts;j++){
                if(cities[i].soldiers_to_move[j] > 0){
                    send_soldier(i, cities[i].dest_id[j], j);
                    if(cities[i].soldiers_to_move[j] == 0){
                        for(int q=j;q<cities[i].dest_counts-1;q++){
                            cities[i].dest_id[q] = cities[i].dest_id[q+1];
                            cities[i].soldiers_to_move[q] = cities[i].soldiers_to_move[q+1];
                        }
                        cities[i].dest_counts--;
                        j--;
                    }
                }
            }
        }
        if(potion_state[cities[i].team] == 4){
            if( start_ticks - cities[i].last_tick_for_growth > 1000/cities[i].growth_rate
            && cities[i].team != 0){
                generate_soldier(1, cities[i].team, cities[i].id);
                cities[i].last_tick_for_growth = start_ticks;
            }
        }else if(potion_state[cities[i].team] == 6){
            if(cities[i].soldier_counts < max_grow_soldier
            && start_ticks - cities[i].last_tick_for_growth > 200/cities[i].growth_rate
            && cities[i].team != 0){
                generate_soldier(1, cities[i].team, cities[i].id);
                cities[i].last_tick_for_growth = start_ticks;
            }
        }else{
            if(cities[i].soldier_counts < max_grow_soldier
            && start_ticks - cities[i].last_tick_for_growth > 1000/cities[i].growth_rate
            && cities[i].team != 0){
                generate_soldier(1, cities[i].team, cities[i].id);
                cities[i].last_tick_for_growth = start_ticks;
            }
        }
    }
}

//this function will be called by draw_map() and draws camps
void draw_camps(SDL_Renderer* renderer){
    SDL_Rect r;
    r.h=40;
    r.w=40;
    char* buffer = (char*) malloc(sizeof(char) * 20);
    for(int i=0;i<cities_count;i++){
        int x = cities[i].x * map_cell_side + map_start_x;
        int y = cities[i].y * map_cell_side + map_start_y;
        r.x = x; 
        r.y = y;
         switch(cities[i].team){
                case 0:SDL_RenderCopyEx(renderer, brown_camp, NULL,&r, 0, NULL, SDL_FLIP_HORIZONTAL);break;
                case 1:SDL_RenderCopyEx(renderer, red_camp, NULL,&r, 0, NULL, SDL_FLIP_HORIZONTAL);break;
                case 2:SDL_RenderCopyEx(renderer, blue_camp, NULL,&r, 0, NULL, SDL_FLIP_HORIZONTAL);break;
                case 3:SDL_RenderCopyEx(renderer, green_camp, NULL,&r, 0, NULL, SDL_FLIP_HORIZONTAL);break;
                case 4:SDL_RenderCopyEx(renderer, gray_camp, NULL,&r, 0, NULL, SDL_FLIP_HORIZONTAL);break;
        }
        sprintf(buffer, "%d", cities[i].soldier_counts);
        stringRGBA(renderer, x +20, y + 3, buffer, 255,255,255,255);
    }
    draw_standing(renderer);
    free(buffer);
}
void draw_standing(SDL_Renderer* renderer){
    int sigma_soldier[5];
    for(int i=0;i<5;i++) {teams_standing[i] =-1; sigma_soldier[i] = 0;}
    for(int i=0;i<cities_count;i++){
        teams_standing[cities[i].team] = cities[i].team;
        sigma_soldier[cities[i].team]+= cities[i].soldier_counts;
    }
    sigma_soldier[0] = -1;
    for(int i=0;i<soldiers_count;i++) {
        if(soldiers[i].dest_x != -1){
             sigma_soldier[soldiers[i].team]++;
             teams_standing[soldiers[i].team] = soldiers[i].team;
        }
    }
    for(int i=0;i<5;i++){
        for(int j=i;j<5;j++){
            if(sigma_soldier[i] < sigma_soldier[j]){
                int temp = sigma_soldier[i];
                sigma_soldier[i] = sigma_soldier[j];
                sigma_soldier[j] = temp;
                temp = teams_standing[i];
                teams_standing[i] = teams_standing[j];
                teams_standing[j] = temp;
            }
        }
    }
    int current_active =0;
    SDL_Rect r;
    for(int i=0;i<players_count;i++){
        if(teams_standing[i] != -1){
            char count[5];
            sprintf(count,"%d",sigma_soldier[i]);
            if(start_ticks % 1000 > 995) printf("%d\n", sigma_soldier[i]);
            r.x = 800;
            r.y = 80+80*current_active;
            r.h = r.w = 70;
            switch (teams_standing[i])
            {
                case 1:{SDL_RenderCopyEx(renderer, red_camp, NULL,&r, 0, NULL, SDL_FLIP_NONE);}break;
                case 2:{SDL_RenderCopyEx(renderer, blue_camp, NULL,&r, 0, NULL, SDL_FLIP_NONE);}break;
                case 3:{SDL_RenderCopyEx(renderer, green_camp, NULL,&r, 0, NULL, SDL_FLIP_NONE);}break;
                case 4:{SDL_RenderCopyEx(renderer, gray_camp, NULL,&r, 0, NULL, SDL_FLIP_NONE);}break;
                default:break;
            }
            current_active++;
            stringRGBA(renderer, r.x+50, r.y,count,0,0,0,255);
        }
    }
}
#endif