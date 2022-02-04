#ifndef MENU_C
#define MENU_C
#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.c" 
#include "io.c"
#include <SDL2/SDL_ttf.h>
Button  buttons[4];
int buttons_count = 2;
void init_buttons(){
    for(int i=0;i<buttons_count;i++){
        buttons[i].x = 800;
        buttons[i].y = 20 + 80*i;
        buttons[i].w =175;
        buttons[i].h = 70;
        buttons[i].caption = (char*) malloc(sizeof(char) * 50);
    }
    sprintf(buttons[0].caption , "Play");
    sprintf(buttons[1].caption , "Add User");
}
void draw_buttons(SDL_Renderer* renderer){
    SDL_Color black_color = {0,0,0};
    SDL_Color blue_color = {0,0,255};
    SDL_Rect main_button_rect = {.x=0 , .y = 0, .w = window_width, .h = window_height};
    SDL_Rect text_rect;
    SDL_RenderCopyEx(renderer, bg_menu, NULL, &main_button_rect, 0 ,NULL, 0); 
    for(int i=0;i<buttons_count;i++){
        main_button_rect.x = buttons[i].x;
        main_button_rect.y = buttons[i].y;
        main_button_rect.w = buttons[i].w;
        main_button_rect.h = buttons[i].h;
        //mouse hover
        SDL_Texture* text_texture;
        if(current_mouse_x - main_button_rect.x <= main_button_rect.w && current_mouse_x - main_button_rect.x >= 0
        && current_mouse_y - main_button_rect.y <= main_button_rect.h && current_mouse_y - main_button_rect.y >= 0 ){
           text_texture = getTextTexture(renderer, "./files/fonts/liber.ttf",buttons[i].caption, blue_color);
        }else{
            text_texture = getTextTexture(renderer, "./files/fonts/liber.ttf",buttons[i].caption, black_color);
        }
        text_rect = main_button_rect;
        SDL_QueryTexture(text_texture, NULL, NULL, &text_rect.w, &text_rect.h);
        text_rect.x += (main_button_rect.w - text_rect.w)/2;
        text_rect.y += (main_button_rect.h - text_rect.h)/2;
        SDL_RenderCopyEx(renderer, btn_main, NULL, &main_button_rect, 0 ,NULL, 0); 
        SDL_RenderCopyEx(renderer, text_texture, NULL, &text_rect, 0 ,NULL, 0); 
        SDL_DestroyTexture(text_texture);
    }
}
void detect_click(int x, int y){
    for(int i=0;i<buttons_count;i++){
        if( x - buttons[i].x <= buttons[i].w && x - buttons[i].x >= 0 
        && y - buttons[i].y <= buttons[i].h && y - buttons[i].y >= 0){
            switch(i){
                case 0:{
                    //play button
                    window_state = 2;
                }break;
                default: break;
            }
        }
    }
}
#endif