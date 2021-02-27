#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "map.h"

//#define DEBUG

#define MAP_W 41
#define MAP_H 41


int main(int argc, char *argv[]){
    struct map *ma = newMap(MAP_W, MAP_H);     
    
    generateMaze(ma);
    delMap(ma);

    return 0;
}

