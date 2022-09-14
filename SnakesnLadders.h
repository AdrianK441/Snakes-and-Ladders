#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

typedef struct Tile {
    int tile_num;
    struct Tile *next;
    struct Tile *ladder;
    struct Tile *snake;
    int has_link;
}Tile;

typedef struct Board {
    struct Tile *start;
    struct Player* players;
    int tiles;
}Board;

typedef struct Player {
    int player_Number;
    struct Tile *position_Tile;
}Player;

typedef struct Game {
    int winner;
    int moves;
    int snake_hits;
    int ladder_hits;
}Game;