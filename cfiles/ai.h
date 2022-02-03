#ifndef AI_H
#define AI_H
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.c"
#include "camps.c"
//prototypes
int is_city_under_attack(int city_index);
int is_city_attacking(int city_index);
int distance_calculator(int* base_cities_index,int city_count, int dest_city_index);
int soldier_counter(int* city_indexes, int city_count);
void attack(int base_id, int dest_id);
int team_city_counter(int team);
int* team_city_finder(int city_count, int team);
int delta_soldiers(int* base_cities_index, int city_count, int dest_index);
int extract_city_from_bitmask(int* out, int bitmask, int* city_indexes, int city_count);
long long scoring(int* city_indexes, int city_count, int destination_index);
void ai();
void attack(int base_id, int dest_id);

//scoring: this function checks that a city is under attack or no
int is_city_under_attack(int city_index){
    for(int i=0;i<soldiers_count;i++){
        if(soldiers[i].dest_x != -1 && soldiers[i].city_id == cities[city_index].id){
            return 1;
        }
    }
    return 0; 
}

//scoring: this function checks that a city is attacking or no
int is_city_attacking(int city_index){
    return cities[city_index].dest_counts != 0 ? 1 : 0;
}

//scoring: this function return manhattan distance of two city
int distance_calculator(int* base_cities_index,int city_count, int dest_city_index){
    int out = 0;
    for(int i=0;i<city_count;i++){
        out += abs(cities[i].x - cities[dest_city_index].x);
        out += abs(cities[i].y - cities[dest_city_index].y);
    }
    return out;
}

//scoring: this function returns cities[city_index].soldiers_count
int soldier_counter(int* city_indexes, int city_count){
    int out =0 ;
    for(int i=0;i<city_count;i++){
        out+= cities[city_indexes[i]].soldier_counts;
    }
    return out;
}

//this function counts cities that belongs to [team]
int team_city_counter(int team){
    int out = 0;
    for(int i=0;i<cities_count;i++){
        if(cities[i].team == team && cities[i].dest_counts == 0) out++;
    }
    return out;
}

//this function returns a int* that contains city_indexes of [team]
int* team_city_finder(int city_count, int team){
    int* out = (int*) malloc(sizeof(int) * city_count);
    int iterator = 0;
    for(int i = 0;i < cities_count; i++){
        if(cities[i].team == team && cities[i].dest_counts == 0){
            out[iterator] = i;
            iterator++;
        }
    }
    return out;
}

//this function returns sigma (home soldiers) - enemy_soldiers
int delta_soldiers(int* base_cities_index, int city_count, int dest_index){
    int out = 0;
    for(int i = 0;i<city_count;i++){
        out += cities[ base_cities_index[i] ].soldier_counts;
    }
    out -= cities[dest_index].soldier_counts;
    return out;
}

// parameters for scoring:
// distance (negative)
// is_attacking (city.dest_id != -1) (positive)
// soldiers_count (negative)
// cities count to attack from (negative)
// sigma soldiers - dest_soldiers (negative) 

//this function recives a bitmask and returns count of 1s in bitmask and fill out
int extract_city_from_bitmask(int* out, int bitmask, int* city_indexes, int city_count){
    int count = 0;
    for(int i=0;i < city_count; i++){
        if ((1LL << i) & bitmask){
            out[count] = city_indexes[i];
            count++;
        }
    }
    return count;
}

// this function scores a attack
long long scoring(int* city_indexes, int city_count, int destination_index){
    int sigma_distance = distance_calculator(city_indexes, city_count, destination_index);
    int is_attacking = is_city_attacking(destination_index);
    int dest_soldiers = cities[destination_index].soldier_counts;
    int our_cities_count = city_count;
    int delta_soldier = delta_soldiers(city_indexes, city_count, destination_index);

    return (-1 * sigma_distance) + (50 * is_attacking) + (-1 * dest_soldiers) + (-10 * our_cities_count) + (-20 * delta_soldier);
}


void ai(){
    int* city = (int*) malloc(sizeof(int) * 50);
    int* final_cities = (int*) malloc(sizeof(int) * 50);
    int final_cities_count = 0;
    int final_dest = -1;
    for(int team_i = 1;team_i <= players_count; team_i++){
        if( team_i != player_id && start_ticks > ai_tick[team_i] * 3000){
            //finding all cities that belongs to the team
            int city_count = team_city_counter(team_i);
            int* team_city = team_city_finder(city_count, team_i);

            int bitmask = 1LL<<city_count;
            
            //valid attack?
            int flag_to_attack = 0;
            long long int max_score = 0;
            long long int score = 0;

            //all possible attacks
            for(int i=0;i < bitmask;i++){
                int count_of_city = extract_city_from_bitmask(city, i, team_city, city_count);
                int soldiers_count_on_cities = soldier_counter(city, count_of_city);
                for(int j=0; j < cities_count; j++){
                    if(cities[j].team != team_i && is_city_under_attack(j) == 0 &&cities[j].soldier_counts + 5< soldiers_count_on_cities){
                        score = scoring(city, count_of_city, j);
                        if(final_dest == -1){
                            max_score = score;
                            final_dest = j;
                            final_cities_count = count_of_city;
                            copy_int_array(city, final_cities, final_cities_count);
                        }else if(score > max_score){
                            max_score = score;
                            final_dest = j;
                            final_cities_count = count_of_city;
                            copy_int_array(city, final_cities, final_cities_count);
                        }
                    }                      
                }
            }
            if(final_dest != -1){
                for(int i=0;i<final_cities_count;i++){
                    attack(cities[final_cities[i]].id, cities[final_dest].id);
                }
                ai_tick[team_i] += rand()%5;
            }
            final_dest = -1;
        }
    }
}

//this function applies a attack
void attack(int base_id, int dest_id){
    int base_index = id_to_city_index(base_id);
    int dest_index = id_to_city_index(dest_id);
    if(cities[base_index].team == 0){
       //invalid attack
    }else{
        cities[base_index].dest_counts++;
        if(cities[base_index].max_dest_counts < cities[base_index].dest_counts + 1){
            cities[base_index].dest_id = (int*) realloc(cities[base_index].dest_id, sizeof(int) * (cities[base_index].dest_counts + 1));
            cities[base_index].soldiers_to_move = (int*) realloc(cities[base_index].soldiers_to_move, sizeof(int) * (cities[base_index].dest_counts + 1));
            cities[base_index].max_dest_counts = cities[base_index].dest_counts;
        }
        int soldiers_to_move_count = 0;
        for(int i=0;i < cities[base_index].dest_counts -1; i++){
            soldiers_to_move_count += cities[base_index].soldiers_to_move[i];
        }
        int iterator = cities[base_index].dest_counts-1;
        cities[base_index].dest_id[iterator] = dest_index;
        cities[base_index].soldiers_to_move[iterator] = cities[base_index].soldier_counts - soldiers_to_move_count;
    }
}

#endif