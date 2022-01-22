#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "consts.c"
#include "map.c"
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