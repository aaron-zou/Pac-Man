#ifndef __Board_h__
#define __Board_h__

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Pacman.h"
#include "Ghost.h"
#include "Sprite.h"
#include "Constants.h"

// Board definition
typedef int8_t board_t[ROWS][COLS];

// ------Board_init()------
// Initialize the beginning state of the board.
// Choose a predetermined board.
// Input: int i {1, 2, 3} - index of board
// Output: none
void Board_init(int i);

// ------Board_numDots()------
// Get the number of dots on the board.
// Input: none
// Output: pointer to number of dots
uint32_t* Board_numDots(void);

// ------Board_setBoard()------
// Used for testing, create a board and pass it in.
// Note that this simply overrides the current board.
// Input: board_t setBoard - board to set
// Output: none
void Board_setBoard(board_t* setBoard);

// ------Board_getBoard()------
// Used for testing, get a pointer to the current board
// Input: none
// Output: board_t* board - pointer to current board instance
board_t* Board_getBoard(void);

// ------Board_isValid()------
// Check if a given boardPosition is valid with respect to
// the physical board (ignoring moving objects).
// Perform out of bounds checks and check if the space is
// not a wall.
// Inputs: boardPos_t boardPos - boardPosition to examine
// Outputs: true if valid
//          false if not valid
bool Board_isValid(boardPos_t boardPos);

// ------Board_getCell()------
// Return the state of the cell given by (row, col).
// Inputs: boardPos_t boardPos - boardPosition to examine
// Outputs: int8_t val - symbolic value of cell
int8_t Board_getCell(boardPos_t boardPos);

// ------Board_setCell()------
// Set the state of the cell given by (row, col).
// Inputs: boardPos_t boardPos - boardPosition to examine
//         int8_t val - value to set the cell to
// Outputs: false if the set was unsuccessful, true otherwise
bool Board_setCell(boardPos_t boardPos, int8_t val);

// ------Board_updateFrames()-----------
// Update the time spent in the current cell for
// Pac-Man and each ghost
// Inputs: none
// Outputs: none
void Board_updateFrames(void);

// ------Board_behind()-------
// Return the direction behind the given direction.
// Inputs: dir - direction
// Outputs behindDir - the opposing "behind" direction
int8_t Board_behind(uint8_t direction);

// ------Board_getEuclideanDist()------
// Get the Euclidean distance between two boardPositions.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: rounded int distance
uint16_t Board_getEuclideanDist(boardPos_t boardPos0, boardPos_t boardPos1);

// ------Board_getManhattanDist()------
// Get the Manhattan distance between two boardPositions.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: int distance
uint16_t Board_getManhattanDist(boardPos_t boardPos0, boardPos_t boardPos1);

// ------Board_validAdjacents()--------
// Return a boolean array indicating whether the adjacent cell
// in each direction N, E, S, W, respectively, is valid.
// Input: boardPos - boardPosition to examine
// Output: ar[4] - whether N, E, S, W are valid (true/false)
bool* Board_validAdjacents(boardPos_t boardPos);

// ------Board_isCorridor()--------
// If a boardPosition and direction are part of a corridor,
// return the only valid direction to subsequently turn.
// Input: boardPos - boardPosition to examine, dir - direction faced
// Output: -1 if not corridor, valid direction otherwise
int8_t Board_isCorridor(boardPos_t boardPos, uint8_t direction);

// ------Board_lookAhead()------
// Get the boardPosition calculated by taking the input 
// boardPosition and "walking" offset steps in the input direction.
// Input:  boardPos    - a boardPosition
//         dir    - direction to travel
//         offset - number of "steps" to walk
// Output: new boardPosition
boardPos_t Board_lookAhead(boardPos_t boardPos, uint8_t dir, uint8_t offset);

// ------Board_posEquals()-----
// Check for equality of two boardPos_t objects.
// Inputs: pos0 - first boardPos, pos1 - second boardPos
// Outputs: true if they are equal, false otherwise
bool Board_posEquals(boardPos_t pos0, boardPos_t pos1);

// ------Board_getDelta()------
// Get the distance between two boardPositions, returning a boardPosition holding
// the row and column differences.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: deltaPos - a boardPosition holding (deltaRow, deltaCol)
boardPos_t Board_getDelta(boardPos_t boardPos0, boardPos_t boardPos1);

// ------Board_addPosns()------
// Add two boardPositions and return a new boardPosition.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: newPos - a new boardPosition representing the vector sum
boardPos_t Board_addPosns(boardPos_t boardPos0, boardPos_t boardPos1);
	
#endif
