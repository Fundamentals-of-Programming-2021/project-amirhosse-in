#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "game.c"

int handleEvents(SDL_Renderer* renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return EXIT;
        if (event.type == SDL_KEYDOWN){
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            if(keys[SDL_SCANCODE_RIGHT]){
                 //Mix_PlayChannel(-1, jumpEffect, 0);
            }
           if(window_state == 3 && keys[SDL_SCANCODE_BACKSPACE]){
                user_name[strlen(user_name)-1]='\0';
                create_new_username_texture(renderer,user_name);
            }
        }
        if(window_state == 3 && event.type == SDL_TEXTINPUT){
            strcat(user_name,event.text.text);
            create_new_username_texture(renderer,user_name);
           // printf("user_name : %s\n", user_name);
        }
        if( event.type == SDL_MOUSEMOTION){
            //event.motion.x and event.motion.y 
            current_mouse_x = event.motion.x;
            current_mouse_y = event.motion.y;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(event.button.button == SDL_BUTTON_LEFT){
                is_mouse_pressed = 1;
                pressed_x = event.button.x;
                pressed_y = event.button.y;
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP){
            if(event.button.button == SDL_BUTTON_LEFT){
                is_mouse_pressed = 0;
                if(window_state == 2){
                    detect_attack(pressed_x, pressed_y, event.button.x, event.button.y);
                }else if(window_state == 1){
                    detect_click(renderer, event.button.x, event.button.y);
                }else if(window_state == 3){
                    detect_click_get_user(renderer, event.button.x, event.button.y);
                }else if(window_state == 4){
                    detect_click_map_menu(renderer, event.button.x, event.button.y,0);
                }else if(window_state == 5){
                    detect_click_standing( event.button.x, event.button.y);
                }
            }
        }
    }
    return 0;
}

int main() {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	SDL_Window* window = SDL_CreateWindow("state.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    messages = (Message*) malloc(sizeof(Message));
    game_generator(renderer);
    init_buttons(renderer);
    init_primary_textures_getuser(renderer);
    int begining_of_time = SDL_GetTicks();
    while (1) {
        start_ticks = SDL_GetTicks();
        if (handleEvents(renderer) == EXIT) break;
    	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    	SDL_RenderClear(renderer);
        if(window_state == 1){
            draw_buttons(renderer);
        }else if(window_state == 2){
            SDL_Rect bg_rect = {.x =0, .y=0, .h= window_height, .w =window_width};
            SDL_RenderCopyEx(renderer, bg_game, NULL, &bg_rect, 180,  NULL, SDL_FLIP_HORIZONTAL);
            draw_map(renderer);
            draw_camps(renderer);
            draw_soldiers(renderer);
            soldier_watcher();
            city_watcher();
            potion_watcher();
            draw_potions(renderer);
            draw_explosions(renderer);
            ai();
            mouse_hover(renderer, current_mouse_x,current_mouse_y,pressed_x, pressed_y, is_mouse_pressed);
            win(renderer);
        }else if(window_state == 3){
            draw_get_user_name(renderer);
        }else if(window_state == 4){
            draw_map_menu(renderer);
        }else if(window_state == 5){
            standing_renderer(renderer);
        }
        messages_watcher(renderer);
    	SDL_RenderPresent(renderer);
        SDL_Delay ( 1000 / FPS );
    }
    clean_memory();
    Mix_FreeChunk(explosion_effect);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
