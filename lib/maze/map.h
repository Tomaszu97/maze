#ifndef MAPLIB_H
#define MAPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include <math.h>

#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define VISITED 0
#define WALL 1

struct map{
    int width;
    int height;
    char *data;
};

extern int current_x;
extern int current_y;

void randInitialize();
int randint();
struct map *newMap(int width, int height);
void delMap(struct map *ma);
void printMap(struct map *ma, int current_x, int current_y);
void prepareMaze(struct map *ma);
int stepMaze(struct map *ma);

#endif
