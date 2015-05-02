#ifndef __Constants_h__
#define __Constants_h__

#include <stdbool.h>
#include <assert.h>
#include "random.h"

// Frame rate
#define FPS 30

// Pac-Man constants
#define LIVES 3

// Directions
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define STILL 7
#define INVALID -1

// Ghost symbolic names
#define BLINKY 0
#define PINKY 1
#define INKY 2
#define CLYDE 3

// Ghost AI behavior
#define CHASE    9
#define SCATTER  10
#define SCARED   11

// Board constants
#define ROWS      36
#define COLS      18
#define CELL_SIZE 6
#define NUM_DOTS  244  // TODO check this number for our boards!! 

// Board cell contents
#define WALL     -1
#define EMPTY    0
#define PACMAN   1
#define GHOST    2
#define FOOD     10
#define BIG_FOOD 50

#endif  /* Constants.h */
