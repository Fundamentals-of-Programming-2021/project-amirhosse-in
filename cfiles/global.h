#ifndef GLOBAL_C
#define GLOBAL_C
#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct Soldier{
    double x;
    double y;
    double speed_x;
    double speed_y;
    double dest_x;
    double dest_y;//equals -1 if be in destination
    int team;
    int city_id;//equals to map[i][j]
    double angle;
} Soldier;

typedef struct Potion{
    int type;
    int time;
    int is_used;
    int team;
    int x;
    int y;
} Potion;

typedef struct City{
    int id;
    int x;
    int y;  
    int team;
    int soldier_counts;
    int* soldiers_to_move;
    int* dest_id;//equals to index of cities array
    int dest_counts;
    int max_dest_counts;
    int growth_rate;
    int last_tick_for_attack;
    int last_tick_for_growth;
} City;

typedef struct Explosion{
    int x;
    int y;
    int state;
    int last_tick;
} Explosion;

typedef struct Button{
    int x;
    int y;
    int h;
    int w;
    char* caption;
} Button;

typedef struct Message{
    char* text;
    int start_time;
    SDL_Color color;
    int need_to_be_freed;
} Message;
const int window_height = 800;
const int window_width = 1000;
int window_state = 3;//1: menu, 2: game, 3: get username, 4: maps, 5: standing
const int FPS = 60 ;
const int EXIT = 12345;
char* user_name;
int user_id = -1;
//map
#define map_width 700   
#define map_height 700
#define map_cell_side 2
#define map_start_x 10
#define map_start_y 10
int map[map_height/map_cell_side][map_width/map_cell_side] = {0};
int maps_count = 0;
//color
SDL_Color red = {255,0,0};
SDL_Color green = {0,255,0};
SDL_Color blue = {0,0,255};
SDL_Color black = {0,0,0};
SDL_Color white = {255,255,255};
SDL_Color my_red = {229,57,53};
SDL_Color my_blue = {48,63, 159};
SDL_Color my_green = {0, 121, 107};
SDL_Color my_gray = {84,110,122};
//City
int teams_standing[5];
City* cities;
int cities_count = 0;
int cities_available[50];
    //textures
    SDL_Texture* brown_camp;
    SDL_Texture* red_camp;
    SDL_Texture* blue_camp;
    SDL_Texture* green_camp;
    SDL_Texture* gray_camp;
//Player and ai
int players_count = 4;
int player_id = 1;
int ai_tick[5]; // for ai's attack
SDL_Texture* winning_caption[4];

//Soldiers
Soldier* soldiers;
int soldiers_count = 0;
int max_soldiers_count = 0;
int primary_anti_soldier_count = 30;
int primary_soldier_count = 30;
int growth_rate_per_second = 1;
double soldier_speed = 9;
int minimum_length_for_collision = 12;
int max_grow_soldier = 60;
int soldiers_size = 30;
int parallel_line_distance = 7; 
    //textures
    SDL_Texture* blue_plane;//planes
    SDL_Texture* red_plane;
    SDL_Texture* green_plane;
    SDL_Texture* gray_plane;

//mouse
int current_mouse_x = 0;
int current_mouse_y = 0;
int is_mouse_pressed = 0;
int pressed_x = 0;
int pressed_y = 0;

//current time
int start_ticks;

//explosion
double explosion_time = 1;
Explosion* explosions;
int explosions_count = 0;
int max_explosions_count = 0;
int explosion_size = 20;

//functions
double abs_double(double a){
    return a<0?-a:a;
}
int sign(double a){
    if(a==0) return 0;
    if(a< 0) return -1;
    if(a>0) return 1;
    return -2;
}
//this functions copy first on second
void copy_int_array(int* first_array, int* second_array, int n){
    for(int i=0;i<n;i++){
        second_array[i] = first_array[i];
    }
}


//audio files
Mix_Chunk *explosion_effect;

//menu
SDL_Texture *bg_menu;
SDL_Texture *bg_game;
SDL_Texture *btn_main;
Button  buttons[4];
SDL_Texture* blue_captions[4];
SDL_Texture* black_captions[4];
int buttons_count = 3;

//ranking
int user_counts = 0;
char** user_names;
int* users_scores;
int logined = 0;
//messages;
int messages_count = 0;
int max_messages_count =0 ;
Message* messages;
void add_new_message(char* message, SDL_Color color, int flag){
    messages_count++;
    if(max_messages_count < 1 + messages_count){
        messages = realloc(messages, sizeof(Message) * (messages_count+1));
        max_messages_count = messages_count;
    }
    messages[messages_count-1].text = message;
    messages[messages_count-1].color = color;
    messages[messages_count-1].start_time = start_ticks;
    messages[messages_count-1].need_to_be_freed = flag;
}
void messages_watcher(SDL_Renderer* renderer){
    for(int i=0;i<messages_count;i++){
        stringRGBA(renderer, 5,5 + 10*i, messages[i].text, messages[i].color.r,  messages[i].color.g, messages[i].color.b,255);
    }
    for(int i=0;i<messages_count;i++){
        if(start_ticks - messages[i].start_time > 2000){
            if(messages[i].need_to_be_freed) free(messages[i].text);
            for(int j=i;j<messages_count-1;j++){
                messages[j] = messages[j+1];
            }
            messages_count--;
            i--;
        }
    }
}

//Potion
SDL_Texture* potions_texture[6];
int potion_state[5];
int potion_impact_time = 6000;
int paused_tick = 0;
int next_potion_tick = 0;
int max_potions_count = 0;
int potions_count = 0;
Potion* potions;
//1: out soldiers speed * 2
//2: fix enemy's soldiers
//3: enemy's soldiers's speed / 2
//4: no limit to create soldier
//5: enemy can't attack 
//6: increase growth rate
#endif