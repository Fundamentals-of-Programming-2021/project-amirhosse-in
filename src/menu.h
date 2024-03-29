#ifndef MENU_H
#define MENU_H
#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.h" 
#include "io.h"
#include <SDL2/SDL_ttf.h>
#include "map_menu.h"
#include "standing.h"

void init_buttons(SDL_Renderer* renderer){
    SDL_Color black_color = {0,0,0};
    SDL_Color blue_color = {0,0,255};
    for(int i=0;i<buttons_count;i++){
        buttons[i].x = 800;
        buttons[i].y = 20 + 80*i;
        buttons[i].w =175;
        buttons[i].h = 70;
        buttons[i].caption = (char*) malloc(sizeof(char) * 50);
    }
    sprintf(buttons[0].caption , "Play");
    sprintf(buttons[1].caption , "Standing");
    sprintf(buttons[2].caption , "Maps");
    for(int i=0;i<buttons_count;i++){
        blue_captions[i] = getTextTexture(renderer, "./assets/fonts/liber.ttf", buttons[i].caption, blue_color);
        black_captions[i] = getTextTexture(renderer, "./assets/fonts/liber.ttf", buttons[i].caption, black_color);
    }
}

void draw_buttons(SDL_Renderer* renderer){
    SDL_Rect main_button_rect = {.x=0 , .y = 0, .w = window_width, .h = window_height};
    SDL_Rect text_rect;
    SDL_RenderCopyEx(renderer, bg_menu, NULL, &main_button_rect, 0 ,NULL, 0); 
    for(int i=0;i<buttons_count;i++){
        main_button_rect.x = buttons[i].x;
        main_button_rect.y = buttons[i].y;
        main_button_rect.w = buttons[i].w;
        main_button_rect.h = buttons[i].h;
        //mouse hover
        text_rect = main_button_rect;
        SDL_RenderCopyEx(renderer, btn_main, NULL, &main_button_rect, 0 ,NULL, 0); 
        if(current_mouse_x - main_button_rect.x <= main_button_rect.w && current_mouse_x - main_button_rect.x >= 0
        && current_mouse_y - main_button_rect.y <= main_button_rect.h && current_mouse_y - main_button_rect.y >= 0 ){
            SDL_QueryTexture(blue_captions[i], NULL, NULL, &text_rect.w, &text_rect.h);
            text_rect.x += (main_button_rect.w - text_rect.w)/2;
            text_rect.y += (main_button_rect.h - text_rect.h)/2;
            SDL_RenderCopyEx(renderer, blue_captions[i], NULL, &text_rect, 0 ,NULL, 0); 
        }else{
            SDL_QueryTexture(black_captions[i], NULL, NULL, &text_rect.w, &text_rect.h);
            text_rect.x += (main_button_rect.w - text_rect.w)/2;
            text_rect.y += (main_button_rect.h - text_rect.h)/2;
            SDL_RenderCopyEx(renderer, black_captions[i], NULL, &text_rect, 0 ,NULL, 0); 
        }
    }
}
void detect_click(SDL_Renderer* renderer, int x, int y){
    for(int i=0;i<buttons_count;i++){
        if( x - buttons[i].x <= buttons[i].w && x - buttons[i].x >= 0 
        && y - buttons[i].y <= buttons[i].h && y - buttons[i].y >= 0){
            switch(i){
                case 0:{//play button
                    if(user_id == -1){
                        add_new_message("You must log in first!", red, 0);
                    }else{
                        window_state = 2;
                        init_buttons_camps(renderer);
                        if(paused_tick != 0){
                            for(int j=0;j<5;j++) ai_tick[j] += (start_ticks - paused_tick)/1000;
                            next_potion_tick += (start_ticks - paused_tick)/1000;
                            for(int j=0;j<potions_count;j++) potions[j].time += (start_ticks - paused_tick);
                            paused_tick=0;
                        }else{
                            for(int j=0;j<5;j++){
                                ai_tick[j]=(start_ticks/1000) + rand()%5;
                                next_potion_tick = rand()%5;
                            }
                        }
                    }
                }break;
                case 1:{//Standing
                    init_textures_standing(renderer);
                    window_state = 5;
                }break;
                case 2:{//maps 
                    init_buttons_map_menu(renderer);
                    window_state = 4;
                }break;
                default: break;
            }
        }
    }
}
#endif