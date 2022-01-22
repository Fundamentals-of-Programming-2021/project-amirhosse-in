#include "game.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "./cfiles/io.c"
#include "./cfiles/objects.c"
#include "./cfiles/map.c"
//protypes
void draw_camps(SDL_Renderer* renderer);
void attack(int base_id, int dest_id);
void draw_soldiers(SDL_Renderer* renderer);
//this function initialize primary thing like generating map and etc.
void game_generator(){
    clock_t aaaa = clock();
    srand(time(NULL));
    int city_count = rand()%5+40;
    soldiers = (Soldier*) malloc(sizeof(Soldier));
    first_init_soldiers();
    //map_generator( &city_count);
    load_map("map1.map");
    remove_border_city();
    cities = find_camps();
    clean_map_from_non_camps_city();
    assign_camps_to_players();
    save_map("map1.map");
    clock_t bbbb = clock();
    double time_s  = (double)(bbbb-aaaa)/CLOCKS_PER_SEC;
    printf("we could generete %d cities in %lf seconds\n", cities_count, time_s);

}
//this function is used for drawing map and border (negative numbers specifies borders) & this function uses id_to_city_index() to determine owner of city
int color_picker(int id){
    int out = 0;
    int x;
    if(id==0) x = -100;
    else{ 
        x = id_to_city_index(abs(id));
        if(cities[x].team != 0) x = cities[x].team * (id < 0? -1 : 1);
        else x = 90 * (id < 0? -1 : 1);
    }
    switch (x)
    {
        case -100:out = 0xff;break;
        case 90:out = 0xff414C6D;break;
        case -90:out = 0xff23273E;break;
        case 1:out=0xff3539E5;break;//red
        case -1:out=0xff1C1CB7;break;
        case 2:out=0xff9F3F30;break;//blue
        case -2:out=0xff7E231A;break;
        case 3:out=0xff6B7900;break;//green
        case -3:out=0xff404D00;break;
        case 4:out=0xff7A6E54;break;//gray
        case -4:out=0xff383226;break;
        default: printf("we found a bug here\n");break;
    }
    return out;
}

//this function draws map on renderer
void draw_map(SDL_Renderer* renderer){
    int x,y;
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            x = j*map_cell_side + map_start_x;
            y = i*map_cell_side + map_start_y; 
            boxColor(renderer, x, y, x+map_cell_side, y+map_cell_side,color_picker(map[i][j]));
        }
    }
    draw_camps(renderer);
    draw_soldiers(renderer);
    soldier_watcher();
    city_watcher();

}
//this function will be called by draw_map() and draws camps
void draw_camps(SDL_Renderer* renderer){
    for(int i=0;i<cities_count;i++){
        int x = cities[i].x * map_cell_side + map_start_x;
        int y = cities[i].y * map_cell_side + map_start_y;
        SDL_Rect r;
        r.x = x; 
        r.y = y;
        r.w = 25;
        r.h = 25;
        SDL_SetRenderDrawColor(renderer, 0,255,0,255);
        SDL_RenderFillRect(renderer, &r);
        char* buffer = (char*) malloc(sizeof(char) * 20);
        sprintf(buffer, "%d", cities[i].id);
        stringRGBA(renderer, x +2 , y + 3, buffer, 0,0,0,255);
    }
}

void draw_soldiers(SDL_Renderer* renderer){
    SDL_Rect soldier_rectangle;
    char* path = (char*) malloc(sizeof(char) * 30);
    for(int i=0;i<soldiers_count;i++){
        if(soldiers[i].dest_x != -1){
            switch(soldiers[i].team){
                case 1:sprintf(path, "./files/red.bmp");break;
                case 2:sprintf(path, "./files/blue.bmp");break;
                case 3:sprintf(path, "./files/green.bmp");break;
                case 4:sprintf(path, "./files/gray.bmp");break;
            }
            soldier_rectangle.x = soldiers[i].x;
            soldier_rectangle.y = soldiers[i].y;
            soldier_rectangle.h = soldiers_size;
            soldier_rectangle.w = soldiers_size;
            SDL_RenderCopyEx(renderer, getImageTexture(renderer,path), NULL,&soldier_rectangle, 45, NULL, SDL_FLIP_HORIZONTAL);
        }
    }
    free(path);
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
            //invalid attack
            printf("it wasn't a valid attack\n");
        }else{
            printf("you attacked from %d to %d\n", map[base_y][base_x],map[dest_y][dest_x]);
            attack(abs(map[base_y][base_x]), abs(map[dest_y][dest_x]));
        }
    }
    else {
        //invalid attack
    }
}

//this function applies a attack
void attack(int base_id, int dest_id){
    int base_index = id_to_city_index(base_id);
    int dest_index = id_to_city_index(dest_id);
    if(cities[base_index].team == 0){
        printf("no way\n");
    }else{
        cities[base_index].dest_id = dest_index;
        cities[base_index].soldiers_to_move = cities[base_index].soldier_counts;
    }
}

