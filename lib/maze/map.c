#include "map.h"

/* initialize rng */
void randInitialize()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/* generate random integer in rance <min;max> */
int randint(int min, int max){
    return min + (rand() % (max-min+1));
}
// int randint(int min, int max)
// {
//     int tmp;
//     if (max >= min)
//         max -= min;
//     else
//     {
//         tmp = min - max;
//         min = max;
//         max = tmp;
//     }
//     return max ? (rand() % max + min) : min;
// }

/* generate empty map */
struct map *newMap(int width, int height)
{
    struct map *ma = (struct map *)malloc(sizeof(struct map));
    ma->width = width;
    ma->height = height;
    int bufsize = width * height;
    ma->data = (char *)malloc(sizeof(char) * bufsize);

    int i;
    for (i = 0; i < bufsize; i++)
    {
        if (i % width == 0 ||
            i % width == width - 1 ||
            i < width ||
            i >= width * (height - 1))
        {
            ma->data[i] = WALL; //borders
        }
        else
        {
            ma->data[i] = WALL; //the rest
        }
    }

    randInitialize();

    return ma;
}

/* purge a map */
void delMap(struct map *ma)
{
    free(ma->data);
    free(ma);
    ma = NULL;
}

/* print ASCII representation of a map to stdout */
/* set current_x and current_y to -1 when not printing current position */
void printMap(struct map *ma, int current_x, int current_y)
{
    system("clear");
    int i, j;
    for (i = 0; i < ma->height; i++)
    {
        for (j = 0; j < ma->width; j++)
        {
            char cell = ma->data[(i * ma->width) + j];
            if (current_x == j && current_y == i)
                printf("XX");
            else if (cell == WALL)
                printf("[]");
            else if (cell == VISITED)
                printf("  ");
        }
        printf("\n");
    }
}

struct stack *st;
int current_x;
int current_y;

/* initialize globals required to calculate the maze */
void prepareMaze(struct map *ma)
{
    st = newStack(ma->width * ma->height);
    current_x = 2 * randint(2, (int)floor(ma->width / 2)) - 1;
    current_y = 2 * randint(2, (int)floor(ma->height / 2)) - 1;
    ma->data[(current_y * ma->width) + current_x] = VISITED;
}

/* run one step of maze generation algorithm */
int stepMaze(struct map *ma)
{
    char edges[4] = {LEFT, UP, RIGHT, DOWN};
    char i;
    for (i = 0; i < 10; i++)
    {
        char from_pos = randint(LEFT, DOWN);
        char to_pos = randint(LEFT, DOWN);
        char temp = edges[to_pos];
        edges[to_pos] = edges[from_pos];
        edges[from_pos] = temp;
    }

    char path_extended = 0;
    for (i = 0; i < 4; i++)
    {
        if (edges[i] == LEFT)
        {
            int neighbor_idx = (current_y * ma->width) + current_x - 2;
            int common_wall_idx = neighbor_idx + 1;
            if (current_x - 2 > -1 && ma->data[neighbor_idx] == WALL)
            {
#ifdef DEBUG
                printf("left\n");
#endif
                push(st, LEFT);
                ma->data[neighbor_idx] = VISITED;
                ma->data[common_wall_idx] = VISITED;
                path_extended = 1;
                current_x -= 2;
                break;
            }
        }
        else if (edges[i] == UP)
        {
            int neighbor_idx = ((current_y - 2) * ma->width) + current_x;
            int common_wall_idx = neighbor_idx + ma->width;
            if (current_y - 2 > -1 && ma->data[neighbor_idx] == WALL)
            {
#ifdef DEBUG
                printf("up\n");
#endif
                push(st, UP);
                ma->data[neighbor_idx] = VISITED;
                ma->data[common_wall_idx] = VISITED;
                path_extended = 1;
                current_y -= 2;
                break;
            }
        }
        else if (edges[i] == RIGHT)
        {
            int neighbor_idx = (current_y * ma->width) + current_x + 2;
            int common_wall_idx = neighbor_idx - 1;
            if (current_x + 2 < ma->width && ma->data[neighbor_idx] == WALL)
            {
#ifdef DEBUG
                printf("right\n");
#endif
                push(st, RIGHT);
                ma->data[neighbor_idx] = VISITED;
                ma->data[common_wall_idx] = VISITED;
                path_extended = 1;
                current_x += 2;
                break;
            }
        }
        else if (edges[i] == DOWN)
        {
            int neighbor_idx = ((current_y + 2) * ma->width) + current_x;
            int common_wall_idx = neighbor_idx - ma->width;
            if (current_y + 2 < ma->height && ma->data[neighbor_idx] == WALL)
            {
#ifdef DEBUG
                printf("down\n");
#endif
                push(st, DOWN);
                ma->data[neighbor_idx] = VISITED;
                ma->data[common_wall_idx] = VISITED;
                path_extended = 1;
                current_y += 2;
                break;
            }
        }
    }

    if (!path_extended)
    {
        if (!isEmpty(st))
        {
            int val = pop(st);
            switch (val)
            {
            case LEFT:
                current_x += 2;
                break;
            case UP:
                current_y += 2;
                break;
            case RIGHT:
                current_x -= 2;
                break;
            case DOWN:
                current_y -= 2;
                break;
            }
#ifdef DEBUG
            printf("backtrack: %d\n", val);
#endif
            // still not completed
            return 1;
        }
        else
        {
                      
            delStack(st);
            //algorithm has finished
            return 0;
        }
    }
}

