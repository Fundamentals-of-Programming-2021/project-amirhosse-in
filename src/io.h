#ifndef IO_H
#define IO_H
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.h"
#include "map.h"
#include <SDL2/SDL_ttf.h>

// prototype 
SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path);
void my_audio_callback(void *userdata, Uint8 *stream, int len);
void load_map(char* path);
void put_number_on_file_pointer(int a, FILE* fp);
void save_map( char* path);
void init_textures(SDL_Renderer* renderer);

//this function resives a int and a file pointer, and put abs(int) in the file pointer
void put_number_on_file_pointer(int a, FILE* fp){
    if(a<0) a*=-1;
    if(a==0){
        fputc('0',fp);
        fputc(',',fp);
        return;
    }
    int digit_count = 0;
    int reversed_number = 0;
    while(a!=0){
        digit_count++;
        reversed_number*=10;
        reversed_number+=a%10;
        a/=10;
    }
    for(int i=0;i<digit_count;i++){
        fputc( (reversed_number%10)+'0', fp);
        reversed_number/=10;
    }
    fputc(',', fp);
}

//this function saves the map
void save_map( char* path){
    FILE* fp =  fopen(path , "w+");
    for(int i=0;i<map_height/map_cell_side;i++){
        for(int j=0;j<map_width/map_cell_side;j++){
            put_number_on_file_pointer(map[i][j], fp);
        }
        fputc('\n', fp);
    }
    fclose(fp);
}

//this function loads the map
void load_map(char* path){
    FILE* fp = fopen(path, "r");
    int temp = 0;
    char temp_char;
    int i=0, j=0;
    while(true){
        temp_char = fgetc(fp);
        if(temp_char == ','){
            map[i][j] = temp;
            temp =0;
            j++;
        }else if(temp_char == '\n'){
            map[i][j] = temp;
            temp =0;
            j=0;
            i++;
        }else if(temp_char == EOF){
            break;
        }else{
             temp *= 10;
             temp += temp_char-'0';
        }
    }
    fclose(fp);
    specify_border();
}

void load_map_count(char* path){
    FILE* f = fopen(path, "r");
    fscanf(f, "%d", &maps_count);
    fclose(f);
}
void save_map_count(char* path){
    FILE* f = fopen(path, "w+");
    fprintf(f, "%d", maps_count);
    fclose(f);
}

void save_map_camps(char* path){
    FILE* f = fopen(path, "w+");
    fprintf(f, "%d\n", players_count);
    fprintf(f, "%d\n", players_count);
    for(int i=0;i<cities_count;i++){
        if(cities[i].team!=0) fprintf(f, "%d\n%d\n", i,cities[i].team);
    }
    fclose(f);
}
void load_map_camps(char* path){
    FILE* f = fopen(path, "r");
    int pc,lines ;
    fscanf(f, "%d%d", &pc, &lines);
    players_count = pc;
    for(int i=0;i<lines;i++){
        int index, team;
        fscanf(f, "%d%d", &index, &team);
        cities[index].team = team;
    }
    fclose(f);
}

void save_user_names(){
    FILE* f = fopen("./assets/usernames.usr", "w+");
    for(int i=0;i < user_counts;i++){
        fprintf(f,"%s\n",user_names[i]);
    }
    fclose(f);
    f = fopen("./assets/scores.scr", "w+");
    for(int i=0;i<user_counts;i++){
        fprintf(f,"%d\n", users_scores[i]);
    }
    fclose(f);
}
void load_user_names(){
    FILE* f = fopen("./assets/usernames.usr", "r");
    user_names = (char**) malloc(sizeof(char*));
    *user_names = (char*) malloc(sizeof(char) * 50);
    char c;
    char* s = malloc(200);
    while(fgets(s, 50, f)){
        s[strcspn(s,"\n")]=0;
        strcpy(user_names[user_counts],s);
        user_counts++;
        user_names = realloc(user_names, sizeof(char*) * (user_counts+1));
        user_names[user_counts] = malloc(sizeof(char) * 50);
    }
    fclose(f);
    users_scores = malloc(sizeof(int) * user_counts);
    f = fopen("./assets/scores.scr", "r");
    for(int i=0;i<user_counts;i++){
        fscanf(f, "%d", users_scores+i);
    }
    fclose(f);
    free(s);
}
void sort_scores(){
    for(int i=0;i<user_counts;i++){
        for(int j=i;j<user_counts;j++){
            if(users_scores[i] < users_scores[j]){
                int temp = users_scores[i];
                users_scores[i] = users_scores[j];
                users_scores[j] = temp;
                char tempo[50] = "";
                strcpy(tempo, user_names[i]);
                strcpy(user_names[i], user_names[j]);
                strcpy(user_names[j], tempo);
            }
        }
    }
}


SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) {
    SDL_Surface *image = SDL_LoadBMP(image_path);
    /* Let the user know if the file failed to load */
    if (!image) {
        printf("Failed to load image at %s: %s\n", image_path, SDL_GetError());
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, image);

    SDL_FreeSurface(image);
    image = NULL;
    return texture;
}
SDL_Texture *getTextTexture(SDL_Renderer *sdlRenderer, char* font_path, char* caption, SDL_Color color){
    TTF_Font *font = TTF_OpenFont(font_path, 32);
    if(!font){
        printf("Failed to load font at %s: %s\n", font_path, SDL_GetError());
        return 0;
    }
    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, caption,color);
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(sdlRenderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    TTF_CloseFont(font);
    return textureText;
}

void init_textures(SDL_Renderer* renderer){
    user_name = (char*) malloc(sizeof(char) * 50);
    user_name[0] = '\0';
    //planes
    blue_plane = getImageTexture(renderer,"./assets/planes/blue.bmp");
    green_plane = getImageTexture(renderer,"./assets/planes/green.bmp");
    red_plane = getImageTexture(renderer,"./assets/planes/red.bmp");
    gray_plane = getImageTexture(renderer,"./assets/planes/gray.bmp");
    //camps
    brown_camp = getImageTexture(renderer,"./assets/camps/brown.bmp");
    blue_camp = getImageTexture(renderer,"./assets/camps/blue.bmp");
    green_camp = getImageTexture(renderer,"./assets/camps/green.bmp");
    gray_camp = getImageTexture(renderer,"./assets/camps/gray.bmp");
    red_camp = getImageTexture(renderer,"./assets/camps/red.bmp");
    //menu
    bg_game = getImageTexture(renderer, "./assets/bgs/game.bmp");
    bg_menu = getImageTexture(renderer, "./assets/bgs/menu.bmp");
    btn_main = getImageTexture(renderer, "./assets/buttons/1.bmp");
    //potions
    for(int i=0;i<6;i++){
        char path[50];
        sprintf(path, "./assets/potions/%d.bmp", i+1);
        potions_texture[i] = getImageTexture(renderer, path);
    }
    //winning caption
    winning_caption[0] = getTextTexture(renderer,"./assets/fonts/liber.ttf" , "Red Won",black);
    winning_caption[1] = getTextTexture(renderer,"./assets/fonts/liber.ttf" , "Blue Won",black);
    winning_caption[2] = getTextTexture(renderer,"./assets/fonts/liber.ttf" , "Green Won",black);
    winning_caption[3] = getTextTexture(renderer,"./assets/fonts/liber.ttf" , "Gray Won",black);
}
void clean_memory(){
    free(user_name);
    SDL_DestroyTexture(blue_plane);
    SDL_DestroyTexture(gray_plane);
    SDL_DestroyTexture(red_plane);
    SDL_DestroyTexture(green_plane);

    SDL_DestroyTexture(red_camp);
    SDL_DestroyTexture(blue_camp);
    SDL_DestroyTexture(green_camp);
    SDL_DestroyTexture(gray_camp);
    SDL_DestroyTexture(brown_camp);

    SDL_DestroyTexture(bg_game);
    SDL_DestroyTexture(bg_menu);
    SDL_DestroyTexture(btn_main);
    for(int i=0;i<6;i++) SDL_DestroyTexture(potions_texture[i]);
    for(int i=0;i<4;i++) SDL_DestroyTexture(winning_caption[i]);

    //Objects
    free(explosions);
    free(messages);
    if(max_potions_count!=0)free(potions);
    for(int j=0;j<cities_count;j++) {free(cities[j].dest_id);free(cities[j].soldiers_to_move);}
    free(cities);
    free(soldiers);
    //io things
    for(int i=0;i<user_counts;i++)free(user_names[i]);
    free(user_names);
    free(users_scores);
    //menu textures
    for(int i=0;i<buttons_count;i++){
        SDL_DestroyTexture(blue_captions[i]);
        SDL_DestroyTexture(black_captions[i]);
        free(buttons[i].caption);
    }

}
#endif