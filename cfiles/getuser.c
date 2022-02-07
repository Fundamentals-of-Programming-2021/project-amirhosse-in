#ifndef GETUSER_C
#define GETUSER_C
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "global.c"
#include "io.c"
SDL_Texture* user_name_texture;
SDL_Rect user_name_rect;
Button  buttons_get_user[4];
SDL_Texture* blue_captions_get_user[4];
SDL_Texture* black_captions_get_user[4];
int buttons_count_get_user = 4;
void init_primary_textures_getuser(SDL_Renderer* renderer){
    for(int i=0;i<buttons_count_get_user;i++){
        buttons_get_user[i].x = 135 + 185*i;
        buttons_get_user[i].y = 500;
        buttons_get_user[i].w =175;
        buttons_get_user[i].h = 70;
        buttons_get_user[i].caption = (char*) malloc(sizeof(char) * 50);
    }
    sprintf(buttons_get_user[0].caption , "Add");
    sprintf(buttons_get_user[1].caption , "Remove");
    sprintf(buttons_get_user[2].caption , "Login");
    sprintf(buttons_get_user[3].caption , "Back");
    for(int i=0;i<buttons_count_get_user;i++){
        blue_captions_get_user[i] = getTextTexture(renderer, "./files/fonts/liber.ttf", buttons_get_user[i].caption, blue);
        black_captions_get_user[i] = getTextTexture(renderer, "./files/fonts/liber.ttf", buttons_get_user[i].caption, black);
    }
}

void create_new_username_texture(SDL_Renderer* renderer, char* name){
    SDL_DestroyTexture(user_name_texture);
    user_name_texture = getTextTexture(renderer, "./files/fonts/liber.ttf", name, black);
    user_name_rect.x = 100;
    user_name_rect.y = 100;
    SDL_QueryTexture(user_name_texture, NULL, NULL, &user_name_rect.w, &user_name_rect.h);
}

void draw_get_user_name(SDL_Renderer* renderer){
    SDL_Rect rect = {.x=0 , .y = 0, .w = window_width, .h = window_height};
    SDL_Rect trect;
    SDL_RenderCopyEx(renderer, bg_menu, NULL, &rect, 0 ,NULL, 0); 
    if(strlen(user_name)){
        SDL_RenderCopyEx(renderer, user_name_texture, NULL, &user_name_rect, 0 ,NULL, 0); 
    }
    for(int i=0;i<buttons_count_get_user;i++){
        rect.x = buttons_get_user[i].x;
        rect.y = buttons_get_user[i].y;
        rect.w = buttons_get_user[i].w;
        rect.h = buttons_get_user[i].h;
        trect = rect;
        SDL_RenderCopyEx(renderer, btn_main, NULL, &rect, 0, NULL, 0);
        if(current_mouse_x - buttons_get_user[i].x <= buttons_get_user[i].w && current_mouse_x - buttons_get_user[i].x >= 0
        && current_mouse_y - buttons_get_user[i].y <= buttons_get_user[i].h && current_mouse_y - buttons_get_user[i].y >= 0){
            SDL_QueryTexture(blue_captions_get_user[i],NULL, NULL, &trect.w, &trect.h);
            trect.x += (rect.w - trect.w)/2;
            trect.y += (rect.h - trect.h)/2;
            SDL_RenderCopyEx(renderer, blue_captions_get_user[i], NULL, &trect, 0 ,NULL, 0); 
        }else{
            SDL_QueryTexture(black_captions_get_user[i],NULL, NULL, &trect.w, &trect.h);
            trect.x += (rect.w - trect.w)/2;
            trect.y += (rect.h - trect.h)/2;
            SDL_RenderCopyEx(renderer, black_captions_get_user[i], NULL, &trect, 0 ,NULL, 0); 
        }
    }
}

void add_user(SDL_Renderer* renderer){
    int flag = 0;
                    for(int j=0;j<user_counts;j++){
                        if(strcmp( user_names[j], user_name) == 0){
                            flag = 1;
                        }
                    }
                    if(!flag){
                        user_counts++;
                        user_names = realloc(user_names, sizeof(char*) * (user_counts+1));
                        user_names[user_counts-1] = malloc(sizeof(char) * 50);
                        strcpy(user_names[user_counts-1], user_name);
                        user_name[0] = '\0';
                        create_new_username_texture(renderer, user_name);
                        add_new_message("User added!", green,0);
                        save_user_names();
                    }else {
                        add_new_message("User already exist!", red,0);
                    }
}

void remove_user(SDL_Renderer* renderer){
    int flag = 0;
                    for(int j=0;j<user_counts;j++){
                        if(strcmp( user_names[j], user_name) == 0){
                            for(int q=j;q<user_counts-1;q++){
                                strcpy(user_names[q], user_names[q+1]);
                            }
                            flag = 1;
                            user_counts--;
                        }
                    }
                    if(flag){
                        user_name[0] = '\0';
                        create_new_username_texture(renderer, user_name);
                        add_new_message("DONE!", green,0);
                        save_user_names();
                    }else{
                        char* buffer=  malloc(sizeof(char) * 50);
                        sprintf(buffer, "%s not found",user_name);
                        add_new_message(buffer, red,1);
                    }
}

void login(SDL_Renderer* renderer){
    int flag =0;
    for(int j=0;j<user_counts;j++){
        if(strcmp(user_names[j], user_name) == 0){
            flag =1;
            user_id = j;
        }
    }
    if(flag) add_new_message("You have successfully logged in!", green , 0);
    else {
        char* buffer =  malloc(sizeof(char) * 50);
        sprintf(buffer, "%s not found!", user_name);
        add_new_message(buffer, red, 1);
    }
}

void detect_click_get_user(SDL_Renderer* renderer, int x, int y){
    for(int i=0;i<buttons_count_get_user;i++){
        if( x - buttons_get_user[i].x <= buttons_get_user[i].w && x - buttons_get_user[i].x >= 0
        &&  y - buttons_get_user[i].y <= buttons_get_user[i].h && y - buttons_get_user[i].y >= 0){
            switch(i){
                case 0:{
                    add_user(renderer);
                }break;
                case 1:{
                    remove_user(renderer);
                }break;
                case 2:{
                    login(renderer);
                }break;
                case 3:{
                    window_state = 1;
                    free(user_name);
                }break;
            }
        }
    }
}

#endif