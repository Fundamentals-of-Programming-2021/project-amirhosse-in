#ifndef MAP_MENU_C
#define MAP_MENU_C
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "global.c"
#include "io.c"
SDL_Texture** blue_textures_map_menu;
SDL_Texture** black_textures_map_menu;
Button *buttons_map_menu;
void init_buttons_map_menu(SDL_Renderer* renderer){
    load_map_count("./maps/maps.map");
    blue_textures_map_menu = malloc(sizeof(SDL_Texture*) * (maps_count+2));
    black_textures_map_menu = malloc(sizeof(SDL_Texture*) * (maps_count+2));
    buttons_map_menu = malloc(sizeof(Button) * (2+maps_count));
    for(int i=0;i<maps_count+2;i++){
        buttons_map_menu[i].x = 790;
        buttons_map_menu[i].y = 10 + 80 * i;
        buttons_map_menu[i].w =175;
        buttons_map_menu[i].h = 70;
        buttons_map_menu[i].caption = (char*) malloc(sizeof(char) * 50);
    }
    for(int i=0;i<maps_count;i++){
        sprintf(buttons_map_menu[i].caption,"map %d" ,i+1);
    }
    sprintf(buttons_map_menu[maps_count].caption,"New");
    sprintf(buttons_map_menu[maps_count+1].caption,"Back");
    for(int i=0;i<maps_count+2;i++){
        blue_textures_map_menu[i] = getTextTexture(renderer, "./files/fonts/liber.ttf", buttons_map_menu[i].caption, blue);
        black_textures_map_menu[i] = getTextTexture(renderer, "./files/fonts/liber.ttf", buttons_map_menu[i].caption, black);
    }
}
void destory_buttons_map_menu(){
    free(blue_textures_map_menu);
    free(black_textures_map_menu);
    for(int i=0;i<maps_count+2;i++){
        free(buttons_map_menu[i].caption);
    }
    free(buttons_map_menu);
}
void draw_map_menu(SDL_Renderer* renderer){
    SDL_Rect rect = {.x=0 , .y = 0, .w = window_width, .h = window_height};
    SDL_Rect trect;
    SDL_RenderCopyEx(renderer, bg_menu, NULL, &rect, 0 ,NULL, 0); 
    //printf("the %d\n", maps_count);
    for(int i=0;i<maps_count+2;i++){
        rect.x = buttons_map_menu[i].x;
        rect.y = buttons_map_menu[i].y;
        rect.w = buttons_map_menu[i].w;
        rect.h = buttons_map_menu[i].h;
        trect = rect;
        SDL_RenderCopyEx(renderer, btn_main, NULL, &rect, 0, NULL, 0);
        if(current_mouse_x - buttons_map_menu[i].x <= buttons_map_menu[i].w && current_mouse_x - buttons_map_menu[i].x >= 0
        && current_mouse_y - buttons_map_menu[i].y <= buttons_map_menu[i].h && current_mouse_y - buttons_map_menu[i].y >= 0){
            SDL_QueryTexture(blue_textures_map_menu[i],NULL, NULL, &trect.w, &trect.h);
            trect.x += (rect.w - trect.w)/2;
            trect.y += (rect.h - trect.h)/2;
            SDL_RenderCopyEx(renderer, blue_textures_map_menu[i], NULL, &trect, 0 ,NULL, 0); 
        }else{
            SDL_QueryTexture(black_textures_map_menu[i],NULL, NULL, &trect.w, &trect.h);
            trect.x += (rect.w - trect.w)/2;
            trect.y += (rect.h - trect.h)/2;
            SDL_RenderCopyEx(renderer, black_textures_map_menu[i], NULL, &trect, 0 ,NULL, 0); 
        }
    }
    draw_map(renderer);
    //draw_camps(renderer);
}
void detect_click_map_menu(SDL_Renderer* renderer, int x, int y, int count){
    for(int i=0;i<maps_count;i++){
        if( x - buttons_map_menu[i].x <= buttons_map_menu[i].w && x - buttons_map_menu[i].x >= 0
        &&  y - buttons_map_menu[i].y <= buttons_map_menu[i].h && y - buttons_map_menu[i].y >= 0){
            char path[50];
            sprintf(path, "./maps/map%d.map", i+1);
            load_map(path);
            load_map_count("./maps/maps.map");
            for(int j=0;j<cities_count;j++) {cities[j].team =0;free(cities[j].dest_id);free(cities[j].soldiers_to_move);}
            for(int j=0;j<50;j++) cities_available[j] =0 ;
            free(cities);
            cities = find_camps();
            sprintf(path, "./maps/camps%d.cmp", i+1);
            load_map_camps(path);
            for(int j=0;j<cities_count;j++)if(cities[j].team!=0) cities[j].soldier_counts=0;
            free(soldiers);
            soldiers_count =0;
            max_soldiers_count=0;
            soldiers = (Soldier*) malloc(sizeof(Soldier));
            first_init_soldiers();
        }
    }
    for(int i=maps_count;i<maps_count+2;i++){
         if( x - buttons_map_menu[i].x <= buttons_map_menu[i].w && x - buttons_map_menu[i].x >= 0
        &&  y - buttons_map_menu[i].y <= buttons_map_menu[i].h && y - buttons_map_menu[i].y >= 0){
            if(i == maps_count){
                destory_buttons_map_menu();
                maps_count++;
                save_map_count("./maps/maps.map");
                init_buttons_map_menu(renderer);
                for(int i=0;i<cities_count;i++) {cities[i].team =0;free(cities[i].dest_id);free(cities[i].soldiers_to_move);}
                for(int i=0;i<50;i++) cities_available[i] =0 ;
                free(cities);
                free(soldiers);
                cities_count=0;
                soldiers_count =0;
                max_soldiers_count=0;
                soldiers = (Soldier*) malloc(sizeof(Soldier));
                int city_count = rand()%5+40;
                map_generator( &city_count);
                remove_border_city();
                cities = find_camps();
                clean_map_from_non_camps_city();
                assign_camps_to_players();
                first_init_soldiers();
                char path[50];
                sprintf(path, "./maps/map%d.map", maps_count);
                save_map(path);
                sprintf(path, "./maps/camps%d.cmp",maps_count);
                save_map_camps(path);
            }else if(i == maps_count+1){
                window_state = 1;
                destory_buttons_map_menu();
            }
        }
    }
}


#endif