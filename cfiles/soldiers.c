#ifndef SOLDIER_C
#define SOLDIER_C
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.c"
#include "io.c"
#include "explosion.h"
#include <SDL2/SDL_mixer.h>
#include "ai.h"
#include "camps.c"

//prototypes
void check_soldiers_collision();
double angle_calculator(double angle, int delta_x, int delta_y);
void generate_soldier(int count, int team, int city_id);
void first_init_soldiers();
void set_soldier_speed_and_dest(int x_src, int y_src, int x_dest, int y_dest, Soldier* soldier);
int soldier_position(Soldier* soldier);
void kill_soldier(Soldier* soldier);
void check_soldier_is_in_dest(Soldier* soldier);
void soldier_watcher();
void send_soldier(int base_city_index, int dest_city_index, int index);
void draw_soldiers(SDL_Renderer* renderer);


//this function generates [count] soldiers and add them to soldiers*
void generate_soldier(int count, int team, int city_id){
    if(max_soldiers_count < count+soldiers_count){
        soldiers = (Soldier*) realloc(soldiers, sizeof(Soldier) * ( (count + soldiers_count) ) );    
        max_soldiers_count +=  (count + soldiers_count) - max_soldiers_count;
    }
    //soldiers = (Soldier*) realloc(soldiers, sizeof(Soldier) * (count + soldiers_count) );
    int city_index = id_to_city_index(city_id);
    for(int i=0;i < count;i++){
        soldiers[soldiers_count].team = cities[city_index].team;
        soldiers[soldiers_count].x = cities[city_index].x * map_cell_side + map_start_x;
        soldiers[soldiers_count].y = cities[city_index].y * map_cell_side + map_start_y;
        soldiers[soldiers_count].city_id = city_id;
        soldiers[soldiers_count].speed_x = 0;
        soldiers[soldiers_count].speed_y = 0;
        soldiers[soldiers_count].dest_y  = -1;
        soldiers[soldiers_count].dest_x  = -1;
        soldiers[soldiers_count].angle = 0;
        cities[city_index].soldier_counts++;
        soldiers_count++;
    }
}

//this function will be called just once to create primary soldiers 
void first_init_soldiers(){
    for(int i=0;i<cities_count;i++){
        if(cities[i].team > 0){
            generate_soldier(primary_soldier_count, cities[i].team, cities[i].id);
        }
    }
}

//this function recives a Soldier* and it's destination and set's it's speed
void set_soldier_speed_and_dest(int x_src, int y_src, int x_dest, int y_dest, Soldier* soldier){
    x_src *= map_cell_side; x_src += map_start_x;
    y_src *= map_cell_side; y_src += map_start_y;
    x_dest *= map_cell_side; x_dest += map_start_x;
    y_dest *= map_cell_side; y_dest += map_start_y;
    soldier->x = x_src;
    soldier->y = y_src;
    soldier->dest_x = x_dest;
    soldier->dest_y = y_dest;
    double delta_x = x_dest - x_src;
    double delta_y = y_dest - y_src;
    if(delta_x == 0){
        soldier->speed_x =0;
        soldier->speed_y = (delta_y>0?soldier_speed:-soldier_speed);
        soldier->angle  = (delta_y>0?180:0);
    }else if(delta_y == 0){
        soldier->speed_x = delta_x > 0? soldier_speed:-soldier_speed;
        soldier->speed_y = 0;
        soldier->angle = delta_x>0?90:-90;
    }else{
        double abs_y = delta_y > 0 ? delta_y : -delta_y;
        double abs_x = delta_x > 0 ? delta_x : -delta_x;
        double angle = atan(abs_y / abs_x);
        soldier->speed_y = soldier_speed * sin(angle);
        soldier->speed_x = soldier_speed * cos(angle);
        soldier->speed_y *= (delta_y > 0? 1 : -1);
        soldier->speed_x *= (delta_x > 0? 1 : -1);
        soldier->angle = angle_calculator(angle, delta_x, delta_y);
    }
    //printf("%lf %lf\n", soldier->speed_x, soldier->speed_y);
}

//this function returns index of a soldier in soldiers*
int soldier_position(Soldier* soldier){
    for(int i=0;i<soldiers_count;i++){
        if(soldiers+i == soldier) return i;
    }
    return -1;
}

//this function kills a soldier and sort soldiers*
void kill_soldier(Soldier* soldier){
    int pos = soldier_position(soldier);
    for(int i=pos;i<soldiers_count-1;i++)
    {
        soldiers[i] = soldiers[i+1];
    }
    soldiers_count -= 1;
}

//check soldier is in destination and does necessary things
void check_soldier_is_in_dest(Soldier* soldier){
    if( abs_double (soldier->x - soldier->dest_x) < minimum_length_for_collision &&  abs_double (soldier->y - soldier->dest_y) < minimum_length_for_collision ){
        soldier->dest_x = -1;
        soldier->dest_y = -1;
        soldier->speed_x= 0;
        soldier->speed_y= 0;
        int city_index = id_to_city_index(soldier->city_id);
        if(cities[city_index].team == 0){
            cities[city_index].soldier_counts -= 1;
            int temp_color = soldier->team;
            kill_soldier(soldier);
            if(cities[city_index].soldier_counts == -1){
                cities[city_index].team = temp_color;
                cities[city_index].soldier_counts = 0;
            }
        }else if(cities[city_index].team == soldier->team){
            cities[city_index].soldier_counts += 1;
        }else{
            if(cities[city_index].soldier_counts > 0){
                cities[city_index].soldier_counts-=1;
                kill_soldier(soldier);
                for(int i=0;i<soldiers_count;i++){
                    if(soldiers[i].team == cities[city_index].team && soldiers[i].city_id == cities[city_index].id && soldiers[i].dest_x == -1){
                        kill_soldier(soldiers+i);
                        break;
                    }
                }
            }else {
                cities[city_index].soldier_counts = 1;
                cities[city_index].team = soldier->team;
				cities[city_index].dest_counts = 0;
            }
        }
    }
}

//this function moves all soldiers (if their speed != 0) and checks they are in destination and checks conflicts
void soldier_watcher(){
    int fix_flag =0;
    int turtle_flag =0;
    for(int i=0;i<5;i++) {
        if(potion_state[i] == 2) fix_flag++;
        if(potion_state[i] == 3) turtle_flag++;
    }
    for(int i=0; i<soldiers_count ;i++){
        if (soldiers[i].speed_x != 0 || soldiers[i].speed_y != 0){
            if(potion_state[soldiers[i].team] == 1){//potion 1
                soldiers[i].x += 2*soldiers[i].speed_x;
                soldiers[i].y += 2*soldiers[i].speed_y;
            }else if(fix_flag != 0){
                if((fix_flag == 1 && potion_state[soldiers[i].team] != 2) || (fix_flag > 1)){
                    //fixed soldier
                }else{
                    soldiers[i].x += soldiers[i].speed_x;
                    soldiers[i].y += soldiers[i].speed_y;
                }
            }else if(turtle_flag != 0){
                if((turtle_flag == 1 && potion_state[soldiers[i].team] != 3) || (turtle_flag > 1)){
                    //turtle soldier
                    soldiers[i].x += 0.5*soldiers[i].speed_x;
                    soldiers[i].y += 0.5*soldiers[i].speed_y;
                }else{
                    soldiers[i].x += soldiers[i].speed_x;
                    soldiers[i].y += soldiers[i].speed_y;
                }
            }
            else{
                soldiers[i].x += soldiers[i].speed_x;
                soldiers[i].y += soldiers[i].speed_y;
            }
            check_soldier_is_in_dest(&soldiers[i]);
        }
    }
    check_soldiers_collision();
}

//this function checks for soldiers collision (and removes them)
void check_soldiers_collision(){
    Soldier* first_soldier;
    Soldier* second_soldier;
    int first_soldiers_count = soldiers_count;
    for(int i=0;i<soldiers_count;i++){
        for(int j=i+1;j<soldiers_count;j++){
            first_soldier = &soldiers[i];
            second_soldier = &soldiers[j];
            if(abs_double(first_soldier->x - second_soldier->x) < minimum_length_for_collision 
            && abs_double(first_soldier->y - second_soldier->y) < minimum_length_for_collision
            && first_soldier->team != second_soldier->team
            && first_soldier->dest_x != -1 && second_soldier->dest_x != -1){
                create_explosion(first_soldier->x, first_soldier->y);
                kill_soldier(first_soldier);
                kill_soldier(second_soldier-1);
                Mix_PlayChannel(-1, explosion_effect, 0);
            }
        }
    }
}

//this function apply attack for 1,2 or more soldier in parallel lines
void send_soldier(int base_city_index, int dest_city_index, int index){
    //(X -  SIGN(DELTA_Y) * SIN(alpha) * L, Y + SIGN(DELTA_X) * COS(alpha) * L
    int X_b = cities[base_city_index].x,
        Y_b = cities[base_city_index].y,
        X_d = cities[dest_city_index].x, 
        Y_d = cities[dest_city_index].y;
    double delta_x = cities[dest_city_index].x - cities[base_city_index].x;
    double delta_y = cities[dest_city_index].y - cities[base_city_index].y;
    double abs_delta_x = abs_double(delta_x);
    double abs_delta_y = abs_double(delta_y);
    double alpha = delta_x != 0 ? atan(abs_delta_y / abs_delta_x) : 1.68;
    int to_move = cities[base_city_index].soldiers_to_move[index] > 2 ? 3 : cities[base_city_index].soldiers_to_move[index];
    int flag = 0;
    Soldier *first_soldier, *second_soldier, *third_soldier;
    for(int i=0;i<soldiers_count;i++){
        if( (soldiers+i)->city_id == cities[base_city_index].id && (soldiers+i)->dest_x == -1 ){
            soldiers[i].city_id = cities[dest_city_index].id;
            if(flag == 0) {
                first_soldier = soldiers+i;
                flag++;
                if(to_move == 1)break;
            }
            else if(flag == 1){
                second_soldier = soldiers+i;
                flag++;
                if(to_move == 2)break;
            }
            else if(flag == 2){
                third_soldier = soldiers+i;
                if(to_move == 3)break;
            }
        }
    }
    double dx = sign(delta_y) * parallel_line_distance * sin(alpha);
    double dy = sign(delta_x) * parallel_line_distance * cos(alpha);
    if(to_move > 2){
        set_soldier_speed_and_dest(X_b - dx, Y_b + dy, X_d - dx, Y_d + dy, first_soldier);
        set_soldier_speed_and_dest(X_b , Y_b , X_d , Y_d , second_soldier);
        set_soldier_speed_and_dest(X_b + dx, Y_b - dy, X_d + dx, Y_d - dy, third_soldier);
        cities[base_city_index].soldiers_to_move[index] -= 3;
        cities[base_city_index].soldier_counts -= 3;
    }else if(to_move == 2){
        set_soldier_speed_and_dest(X_b - dx, Y_b + dy, X_d - dx, Y_d + dy, second_soldier);
        set_soldier_speed_and_dest(X_b + dx, Y_b - dy, X_d + dx, Y_d - dy, first_soldier); 
        cities[base_city_index].soldiers_to_move[index] -= 2;
        cities[base_city_index].soldier_counts -= 2;
    }else if(to_move ==1){
        set_soldier_speed_and_dest(X_b , Y_b , X_d , Y_d , first_soldier);
        cities[base_city_index].soldiers_to_move[index] -= 1;
        cities[base_city_index].soldier_counts -= 1;
    }
}

//this function calculate appriate angle for soldiers
double angle_calculator(double angle, int delta_x, int delta_y){
    angle*=180/3.14;
    if(delta_x > 0 && delta_y < 0){
        return 90-angle;
    }else if(delta_x >0 && delta_y > 0){
        return 90+angle;
    }else if(delta_x<0 && delta_y < 0){
        return angle-90;
    }else return -angle-90;
    return 0;
}

void draw_soldiers(SDL_Renderer* renderer){
    SDL_Rect soldier_rectangle;
    char* path = (char*) malloc(sizeof(char) * 30);
    for(int i=0;i<soldiers_count;i++){
        if(soldiers[i].dest_x != -1){
            soldier_rectangle.x = soldiers[i].x;
            soldier_rectangle.y = soldiers[i].y;
            soldier_rectangle.h = soldiers_size;
            soldier_rectangle.w = soldiers_size;
            switch(soldiers[i].team){
                case 1:SDL_RenderCopyEx(renderer, red_plane, NULL,&soldier_rectangle, soldiers[i].angle, NULL, SDL_FLIP_HORIZONTAL);break;
                case 2:SDL_RenderCopyEx(renderer, blue_plane, NULL,&soldier_rectangle, soldiers[i].angle, NULL, SDL_FLIP_HORIZONTAL);break;
                case 3:SDL_RenderCopyEx(renderer, green_plane, NULL,&soldier_rectangle, soldiers[i].angle, NULL, SDL_FLIP_HORIZONTAL);break;
                case 4:SDL_RenderCopyEx(renderer, gray_plane, NULL,&soldier_rectangle, soldiers[i].angle, NULL, SDL_FLIP_HORIZONTAL);break;
            }
        }
    }
    free(path);
}

#endif