#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "consts.c"



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
    return cities[city_index].dest_id != -1 ? 1 : 0;
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
int soldier_counter(int city_index){
    return cities[city_index].soldier_counts;
}

//this function counts cities that belongs to [team]
int team_city_counter(int team){
    int out = 0;
    for(int i=0;i<cities_count;i++){
        if(cities[i].team == team) out++;
    }
    return out;
}

//this function returns a int* that contains city_indexes of [team]
int* team_city_finder(int city_count, int team){
    int* out = (int*) malloc(sizeof(int) * city_count);
    int iterator = 0;
    for(int i = 0;i < cities_count; i++){
        if(cities[i].team == team){
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
        if ((1LL << i) | bitmask){
            out[count] = city_indexes[i];
            count++;
        }
    }
    return count;
}

// this function scores a attack
long long scoring(int* city_indexes, int city_count, int destination_index){
    int* city= = (int*) malloc(sizeof(int) * 50);
    for(int i = 1;i <= players_count; i++){
        if( i != player_id ){
            int city_count = team_city_counter(i);
            int* team_city = team_city_finder(city_count, i);
            int bitmask = 1LL<<city_count;
            for(int i=0;i < bitmask;i++){
                int count_of_city = extract_city_from_bitmask(city, bitmask, team_city, city_count);
                
            }
        }
    }
}