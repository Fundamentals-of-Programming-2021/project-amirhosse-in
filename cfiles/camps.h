#ifndef CAMPS_H
#define CAMPS_H

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include "global.c" 

int check_is_valid_camp(int row, int column);
City* find_camps();
void assign_camps_to_players();
int team_number_to_city_index(int team);
int id_to_city_index(int map_number);

#endif