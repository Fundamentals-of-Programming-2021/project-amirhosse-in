#ifndef IO_C
#define IO_C
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "global.c"
#include "map.c"
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
    for(int i=0;i<cities_count;i++){
        if(cities[i].team!=0) fprintf(f, "%d\n%d\n", i,cities[i].team);
    }
    fclose(f);
}
void load_map_camps(char* path){
    FILE* f = fopen(path, "r");
    int pc ;
    fscanf(f, "%d", &pc);
    for(int i=0;i<2*pc;i++){
        int index, team;
        fscanf(f, "%d%d", &index, &team);
        cities[index].team = team;
    }
    fclose(f);
}
void save_user_names(){
    FILE* f = fopen("usernames.usr", "w+");
    for(int i=0;i < user_counts;i++){
        fprintf(f,"%s\n",user_names[i]);
    }
    fclose(f);
}
void load_user_names(){
    FILE* f = fopen("usernames.usr", "r");
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
    //planes
    blue_plane = getImageTexture(renderer,"./files/planes/blue.bmp");
    green_plane = getImageTexture(renderer,"./files/planes/green.bmp");
    red_plane = getImageTexture(renderer,"./files/planes/red.bmp");
    gray_plane = getImageTexture(renderer,"./files/planes/gray.bmp");
    //camps
    brown_camp = getImageTexture(renderer,"./files/camps/brown.bmp");
    blue_camp = getImageTexture(renderer,"./files/camps/blue.bmp");
    green_camp = getImageTexture(renderer,"./files/camps/green.bmp");
    gray_camp = getImageTexture(renderer,"./files/camps/gray.bmp");
    red_camp = getImageTexture(renderer,"./files/camps/red.bmp");
    //menu
    bg_game = getImageTexture(renderer, "./files/bgs/game.bmp");
    bg_menu = getImageTexture(renderer, "./files/bgs/menu.bmp");
    btn_main = getImageTexture(renderer, "./files/buttons/1.bmp");
    //potions
    for(int i=0;i<6;i++){
        char path[50];
        sprintf(path, "./files/potions/%d.bmp", i+1);
        potions_texture[i] = getImageTexture(renderer, path);
    }
    //winning caption
    winning_caption[0] = getTextTexture(renderer,"./files/fonts/liber.ttf" , "Red Won",black);
    winning_caption[1] = getTextTexture(renderer,"./files/fonts/liber.ttf" , "Blue Won",black);
    winning_caption[2] = getTextTexture(renderer,"./files/fonts/liber.ttf" , "Green Won",black);
    winning_caption[3] = getTextTexture(renderer,"./files/fonts/liber.ttf" , "Gray Won",black);
}

#endif