// Board.c
// Describes the logical game board.
// Provides useful functions for collision handling.

#include "Board.h"
#include "UART.h"

// Static definitions of possible boards
static board_t board1 = {{0}};
static board_t board2 = {{0}};
static board_t board3 = {{0}};

// Array of pointers to possible boards
static board_t *boards[3] = {&board1, &board2, &board3};

// Logical game board instance
static board_t *board;
static uint32_t numDots;

// ------Board_init()------
// Initialize the beginning state of the board.
// Choose a predetermined board.
// Input: int i {1, 2, 3} - index of board
// Output: none
void Board_init(int i) {
   board = boards[i - 1];
}

// ------Board_numDots()------
// Get the number of dots on the board.
// Input: none
// Output: pointer to number of dots
uint32_t* Board_numDots(void) {
   return &numDots;
}

// ------Board_setBoard()------
// Used for testing, create a board and pass it in.
// Note that this simply overrides the current board.
// Input: board_t setBoard - board to set
// Output: none
void Board_setBoard(board_t* setBoard) {
   memcpy(*board, *setBoard, sizeof(board_t));
}

// ------Board_getBoard()------
// Used for testing, get a pointer to the current board
// Input: none
// Output: board_t* board - pointer to current board instance
board_t* Board_getBoard(void) {
   return board;
}

// ------Board_isValid()------
// Check if a given boardPosition is valid with respect to
// the physical board (ignoring moving objects).
// Perform out of bounds checks and check if the space is
// not a wall.
// Inputs: boardPos_t boardPos - boardPosition to examine
// Outputs: true if valid
//          false if not valid
bool Board_isValid(boardPos_t boardPos) {
   if (boardPos.row < 0 || boardPos.row >= ROWS ||
       boardPos.col < 0 || boardPos.col >= COLS)
      return false;
   return !((*board)[boardPos.row][boardPos.col] == WALL);
}

// ------Board_isWall()------
// Check if a given boardPosition is a wall.
// Inputs: boardPos - boardPosition to examine
// Outputs: true if a wall, false otherwise
bool Board_isWall(boardPos_t boardPos) {
   return Board_getCell(boardPos) == WALL;
}

// ------Board_getCell()------
// Return the state of the cell given by (row, col).
// Inputs: boardPos_t boardPos - boardPosition to examine
// Outputs: int8_t val - symbolic value of cell, -1 if unsuccessful
int8_t Board_getCell(boardPos_t boardPos) {
   if (!Board_isValid(boardPos)) return INVALID;       
   return (*board)[boardPos.row][boardPos.col];
}

// ------Board_setCell()------
// Set the state of the cell given by (row, col).
// Inputs: boardPos_t boardPos - boardPosition to examine
//         int8_t val - value to set the cell to
// Outputs: false if the set was unsuccessful, true otherwise
bool Board_setCell(boardPos_t boardPos, int8_t val) {
   if (boardPos.row < 0 || boardPos.row >= ROWS ||
       boardPos.col < 0 || boardPos.col >= COLS)
      return false;
   (*boards[0])[boardPos.row][boardPos.col] = val;
   return true;
}

// ------Board_behind()-------
// Return the direction behind the given direction.
// Inputs: dir - direction
// Outputs behindDir - the opboardPosing "behind" direction
int8_t Board_behind(uint8_t direction) {
   switch (direction) {
      case NORTH: return SOUTH; 
      case EAST:  return WEST;
      case SOUTH: return NORTH;
      case WEST:  return EAST;
      default: return INVALID;
   }
}

// ------Board_getEuclideanDist()------
// Get the Euclidean distance between two boardPositions.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: rounded int distance, -1 if invalid inputs
uint16_t Board_getEuclideanDist(boardPos_t boardPos0, boardPos_t boardPos1) {
   uint16_t a2 = (boardPos0.row - boardPos1.row) * (boardPos0.row - boardPos1.row);
   uint16_t b2 = (boardPos0.col - boardPos1.col) * (boardPos0.col - boardPos1.col);
   uint16_t dist = (uint16_t)(sqrt(a2 + b2));
   return dist;
}

// ------Board_getManhattanDist()------
// Get the Manhattan between two boardPositions.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: int distance, -1 if invalid inputs
uint16_t Board_getManhattanDist(boardPos_t boardPos0, boardPos_t boardPos1) {
   uint16_t dist = abs(boardPos0.row - boardPos1.row) + abs(boardPos0.col - boardPos1.col);
   return dist;
}

// ------Board_validAdjacents()--------
// Return a boolean array indicating whether the adjacent cell
// in each direction N, E, S, W, respectively, is valid.
// Input: boardPos - boardPosition to examine
// Output: ar[4] - whether N, E, S, W are valid (true/false)
static bool valids[4];
bool* Board_validAdjacents(boardPos_t boardPos) {   
   valids[0] = Board_isValid(Board_lookAhead(boardPos, NORTH, 1));
   valids[1] = Board_isValid(Board_lookAhead(boardPos, EAST, 1)); 
   valids[2] = Board_isValid(Board_lookAhead(boardPos, SOUTH, 1)); 
   valids[3] = Board_isValid(Board_lookAhead(boardPos, WEST, 1)); 
   return valids;
}

// ------Board_isCorridor()--------
// If a boardPosition and direction are part of a corridor,
// return the only valid direction to subsequently turn.
// Input: boardPos - boardPosition to examine, dir - direction faced
// Output: -1 if not corridor, valid direction otherwise
int8_t Board_isCorridor(boardPos_t boardPos, uint8_t direction) {
   bool *validDir = Board_validAdjacents(boardPos);   
   int avoid = Board_behind(direction);
   int numExits = 0;
   
   for (int i = 0; i < 4; i++) {
      numExits += validDir[i];
   }
   
   numExits -= validDir[avoid];
   
   // Check that this is a corridor
   if (numExits != 1) {
      return -1;
   } else {
      // Return the only valid direction
      for (int i = 0; i < 4; i++) {
         if ((i != avoid) && valids[i]) {
            return i;  // index corresponds to N, E, S, W
         }
      }
   }
   
   return -1;  // should never be reached
}

// ------Board_lookAhead()------
// Get the boardPosition calculated by taking the input 
// boardPosition and "walking" offset steps in the input direction.
// Input:  boardPos    - a boardPosition
//         dir    - direction to travel
//         offset - number of "steps" to walk
// Output: new boardPosition
boardPos_t Board_lookAhead(boardPos_t boardPos, uint8_t dir, uint8_t offset) {
   boardPos_t newPos = {boardPos.row, boardPos.col};   
   switch (dir) {
      case NORTH: newPos.row -= offset; break;
      case EAST:  newPos.col += offset; break;
      case SOUTH: newPos.row += offset; break;
      case WEST:   newPos.col -= offset; break;
   }   
   return newPos;
}

// ------Board_posEquals()-----
// Check for equality of two boardPos_t objects.
// Inputs: pos0 - first boardPos, pos1 - second boardPos
// Outputs: true if they are equal, false otherwise
bool Board_posEquals(boardPos_t pos0, boardPos_t pos1) {
   return (pos0.row == pos1.row) && (pos0.col == pos1.col);
}

// ------Board_getDelta()------
// Get the distance between two boardPositions, returning a boardPosition holding
// the row and column differences.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: deltaPos - a boardPosition holding (deltaRow, deltaCol) calculated from boardPos0 - boardPos1
boardPos_t Board_getDelta(boardPos_t boardPos0, boardPos_t boardPos1) {
   boardPos_t deltaPos = {boardPos0.row - boardPos1.row, boardPos0.col - boardPos1.col};
   return deltaPos;
}

// ------Board_addPosns()------
// Add two boardPositions and return a new boardPosition.
// Input:  boardPos0 - a boardPosition, boardPos1 - a second boardPosition
// Output: newPos - a new boardPosition representing the vector sum
boardPos_t Board_addPosns(boardPos_t boardPos0, boardPos_t boardPos1) {
   boardPos_t newPos = {boardPos0.row + boardPos1.row, boardPos0.col + boardPos1.col};
   return newPos;
}
