#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "consts.c"
//prototypes
void check_soldiers_collision();
double angle_calculator(double angle, int delta_x, int delta_y);

//this function is used by find_camps() to check a cell is appropriate for a camp or not
int check_is_valid_camp(int row, int column){
    int base_color = map[row][column];
    int flag = 1;
    for(int i=-10;i < 20;i++){
        for(int j=-10; j< 20;j++){
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
                cities[(cities_count)-1].soldiers_to_move = 0;
                cities[(cities_count)-1].growth_rate = growth_rate_per_second;
                cities[(cities_count)-1].dest_id = -1;
                cities[(cities_count)-1].last_tick_for_attack = 0;
                cities[(cities_count)-1].last_tick_for_growth = 0;
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
//this function recives non zero abs(map[i][j]) and return index of city 
int id_to_city_index(int map_number){
    for(int i=0;i <cities_count;i++){
        if(cities[i].id == map_number ){
            return i;
        }
    }
    return -1;
}

//this function return city index from city team
int team_number_to_city_index(int team){
    for(int i=0;i<cities_count;i++){
        if(cities[i].team == team) return i;
    }
    return -1;
}

//this function generates [count] soldiers and add them to soldiers*
void generate_soldier(int count, int team, int city_id){
    if(max_soldiers_count < count+soldiers_count){
        soldiers = (Soldier*) realloc(soldiers, sizeof(Soldier) * ( (count + soldiers_count) ) );    
        max_soldiers_count +=  (count + soldiers_count) - max_soldiers_count;
    }
    soldiers = (Soldier*) realloc(soldiers, sizeof(Soldier) * (count + soldiers_count) );
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
    soldier->dest_x = x_dest;
    soldier->dest_y = y_dest;
    double delta_x = x_dest - x_src;
    double delta_y = y_dest - y_src;
    if(delta_x == 0){
        soldier->speed_x =0;
        soldier->speed_y = (delta_y>0?3:-3);
        soldier->angle  = (delta_y>0?0:180);
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
}

//this function returns position of a soldier in soldiers*
int soldier_position(Soldier* soldier){
    for(int i=0;i<soldiers_count;i++){
        if(soldiers+i == soldier) return i;
    }
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
                    if(soldiers[i].team == cities[city_index].team){
                        kill_soldier(soldiers+i);
                        break;
                    }
                }
            }else {
                cities[city_index].team = soldier->team;
            }
        }
    }
}

//this function moves all soldiers (if their speed != 0) and checks they are in destination and checks conflicts
void soldier_watcher(){
    Soldier* soldier_ptr;
    for(int i=0; i<soldiers_count ;i++){
        soldier_ptr = &soldiers[i];
        if (soldier_ptr -> speed_x != 0 || soldier_ptr -> speed_y != 0){
            soldier_ptr->x += soldier_ptr->speed_x;
            soldier_ptr->y += soldier_ptr->speed_y;
            check_soldier_is_in_dest(soldier_ptr);
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
            && first_soldier->team != second_soldier->team){
                kill_soldier(first_soldier);
                kill_soldier(second_soldier-1);
            }
        }
    }
}

void city_watcher(){
    for(int i=0;i<cities_count;i++){
        if(cities[i].soldiers_to_move > 0 && start_ticks - cities[i].last_tick_for_attack > 300){
            for(int j=0;j<soldiers_count;j++){
                if( soldiers[j].city_id == cities[i].id && soldiers[j].dest_x == -1){
                    int city_index = cities[i].dest_id;
                    soldiers[j].city_id = cities[city_index].id;
                    set_soldier_speed_and_dest(cities[i].x, cities[i].y, cities[city_index].x, cities[city_index].y, soldiers+j);
                    cities[i].soldiers_to_move -= 1;
                    cities[i].soldier_counts -= 1;
                    cities[i].last_tick_for_attack = start_ticks;
                    if(cities[i].soldiers_to_move == 0) cities[i].dest_id = -1;
                    break;
                }
            }
        }
        if(cities[i].soldier_counts < max_grow_soldier
        && start_ticks - cities[i].last_tick_for_growth > 1000/cities[i].growth_rate
        && cities[i].team != 0){
            generate_soldier(1, cities[i].team, cities[i].id);
            cities[i].last_tick_for_growth = start_ticks;
        }
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

