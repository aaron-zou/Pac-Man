#ifndef __TestBoard_h__
#define __TestBoard_h__

#include "Board.h"

// Test setting and getting the same board
bool TestBoard_testSetBoard(void);

// Test setting and getting the same cell
bool TestBoard_testSetCell(void);

// Test validity checks (out of bounds and wall)
bool TestBoard_testValid(void);

// Test that the current opposite direction is returned
bool TestBoard_testBehind(void);

// Test that the correct distances are returned
bool TestBoard_testEuclidDist(void);
bool TestBoard_testManhattanDist(void);

// Test that the correct cell states are returned
bool TestBoard_testAdjacents(void);

// Test correctness of corridor check
bool TestBoard_testIsCorridor(void);

// Test correctness of lookahead
bool TestBoard_testLookahead(void);

#endif /* TestBoard.h */
