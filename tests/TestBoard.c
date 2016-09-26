// Test Board behavior

#include "TestBoard.h"
#include "UART.h"

// Board instance used for tests
static board_t testBoard = {{0}};

// Test setting and getting the same board
bool TestBoard_testSetBoard(void) {
	int8_t fill = Random() & 0xFF;
   memset(testBoard, fill, sizeof(board_t));  // fill board with all the same random value
	Board_setBoard(&testBoard);
	board_t *result = Board_getBoard();
	
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if ((*result)[i][j] != fill) {
				UART_OutString("Failed testSetBoard.\n");
				return false;
			}
		}
	}
	
	return true;
}

// Test setting and getting the same cell
bool TestBoard_testSetCell(void) {
	Board_init(1);
	
	// Check that get() returns what was set()
	int8_t fill = Random() & 0xFF;
	boardPos_t pos = {0, 0};
	Board_setCell(pos, fill);
	uint8_t val = Board_getCell(pos);    
	if (fill != val) return false;
	
	// Try setting/getting an invalid cell
	fill = Random() & 0xFF;
	pos.row = pos.col = -1;
	if (Board_setCell(pos, fill)) return false;
	if (Board_getCell(pos) != -1) return false;
	return true;
}

// Test validity checks (out of bounds and wall)
bool TestBoard_testValid(void) {
	// Test various (in)valid positions and set wall at an otherwise valid position.
	boardPos_t posns[10] = {{0, 0}, {-1, -1}, {-1, 0}, {ROWS, COLS}, {ROWS - 1, COLS - 1}, 
								   {2, 2}, {INT16_MIN, INT16_MIN}, {ROWS / 2, COLS / 2}, 
									{1, 2}, {1, 1}};
	
	Board_setCell(posns[5], WALL);
	Board_setCell(posns[8], PACMAN);
									
	for (int i = 0; i < 10; i++) {
		bool valid = Board_isValid(posns[i]);
		switch (i) {
			case 0: if (!valid) return false;  break;
			case 1: if (valid)  return false;  break;
			case 2: if (valid)  return false;  break;
			case 3: if (valid)  return false;  break;
			case 4: if (!valid) return false;  break;
			case 5: if (valid)  return false;  break;
			case 6: if (valid)  return false;  break;
			case 7: if (!valid) return false;  break;
			case 8: if (!valid) return false;  break;
			case 9: if (!valid) return false;  break;
		}
	}
	
	return true;
}

// Test that the current opposite direction is returned
bool TestBoard_testBehind(void) {
	if (Board_behind(NORTH) != SOUTH) return false;
	if (Board_behind(EAST) != WEST) return false;
	if (Board_behind(SOUTH) != NORTH) return false;
	if (Board_behind(WEST) != EAST) return false;
	return true;
}

// Test that the correct distances are returned
bool TestBoard_testEuclidDist(void) {
	int y0 = Random() % COLS;
	int y1 = Random() % COLS;
	boardPos_t pos0 = {0, y0};
	boardPos_t pos1 = {0, y1};
	return (Board_getEuclideanDist(pos0, pos1) == abs(y1 - y0));	
}

// Test that the correct distances are returned
bool TestBoard_testManhattanDist(void) {
	boardPos_t pos0 = {Random() % ROWS, Random() % COLS};
	boardPos_t pos1 = {Random() % ROWS, Random() % COLS};
	return (Board_getManhattanDist(pos0, pos1) == 
		abs(pos0.row - pos1.row) + abs(pos0.col - pos1.col));	
}

// Test that the correct cell states are returned
bool TestBoard_testAdjacents(void) {
	// Create expected adjacents
	boardPos_t posns[4] = {{0, 1}, {1, 2}, {2, 1}, {1, 0}};
	int values[4] = {Random() % 4 - 1, Random() % 4 - 1, Random() % 4 - 1, Random() % 4 - 1};
	bool expected[4] = {values[0] != WALL, values[1] != WALL, values[2] != WALL, values[3] != WALL};
	
	Board_setCell(posns[0], values[0]);
	Board_setCell(posns[1], values[1]);
	Board_setCell(posns[2], values[2]);
	Board_setCell(posns[3], values[3]);
	
	boardPos_t test = {1, 1};
	bool *actual = Board_validAdjacents(test);
	
	// Compare actual value
	for (int i = 0; i < 4; i++) {
		if (expected[i] != actual[i]) return false;
	}
	
	// Check an invalid position
	boardPos_t invalidPos = {-1, -1};
	actual = Board_validAdjacents(invalidPos);
	return !(actual[0] || actual[1] || actual[2] || actual[3]);
}

// Test correctness of corridor check
bool TestBoard_testIsCorridor(void) {
	// Construct all four types of corridors
	boardPos_t myPos = {1, 1};
	boardPos_t posns[4] = {{0, 1}, {1, 2}, {2, 1}, {1, 0}};
	int directions[4] = {NORTH, EAST, SOUTH, WEST};
	int corridors[4][4] = {{EMPTY, WALL, EMPTY, WALL}, {WALL, EMPTY, WALL, EMPTY}, 
								  {EMPTY, WALL, EMPTY, WALL}, {WALL, EMPTY, WALL, EMPTY}};
	
	// Create corridor in ith direction
	for (int i = 0; i < 4; i++) {
		Board_setCell(posns[0], corridors[i][0]);		
		Board_setCell(posns[1], corridors[i][1]);		
		Board_setCell(posns[2], corridors[i][2]);		
		Board_setCell(posns[3], corridors[i][3]);
		if (Board_isCorridor(myPos, directions[i]) == -1) return false;
	}
		
	// Construct a non-corridor	
	for (int i = 0; i < 4; i++) {
		Board_setCell(posns[i], WALL);		
	}
	
	if (Board_isCorridor(myPos, NORTH) != -1) return false;
	
	// Check an invalid position
	boardPos_t invalidPos = {-1, -1};
	return Board_isCorridor(invalidPos, NORTH) == -1;
}

// Test correctness of lookahead
bool TestBoard_testLookahead(void) {
	// Randomly generate test cases
	for (int i = 0; i < 20; i++) {
		// Position uniformly distributed on {{-ROWS, ROWS}, {-COLS, COLS}}
		boardPos_t pos = {(Random() % (2 * ROWS)) - ROWS, 
							   (Random() % (2 * COLS)) - COLS};
		int direction = Random() % 4;
		int offset = Random() % ROWS;
								
		// Validate lookahead
		boardPos_t ahead = Board_lookAhead(pos, direction, offset);
		switch (direction) {
			case NORTH: if (ahead.row != (pos.row - offset) || 
								 ahead.col != pos.col)              return false; break;
			case EAST:  if (ahead.row != pos.row || 
								 ahead.col != (pos.col + offset))   return false; break;
			case SOUTH: if (ahead.row != (pos.row + offset) || 
								 ahead.col != pos.col)              return false; break;
			case WEST:  if (ahead.row != pos.row || 
								 ahead.col != (pos.col - offset))   return false; break;		
		}
	}
	return true;
}
