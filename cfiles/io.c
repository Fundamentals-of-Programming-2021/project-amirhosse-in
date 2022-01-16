#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "consts.c"
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

