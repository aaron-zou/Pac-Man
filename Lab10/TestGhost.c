// Test Ghost behavior

#include "TestGhost.h"

// Comprehensive testing of AI behavior
// Assumes correctness of all other ghost methods
bool TestGhost_testNextDir(void) {
	// Test Blinky
	// Corridor case
	
	// Intersection case
	
	// Test Pinky
	// Corridor case
	
	// Intersection case
	
	// Test Inky
	// Corridor case
	
	// Intersection case
	
	
	// Test Clyde
	// Corridor case
	
	// Intersection case
	
	
	return true;
}

// Test that behavior can corectly be set
bool TestGhost_testSetBehavior(void) {
	Ghost_setBehavior(SCARED);
	if (Ghost_getBehavior() != SCARED) return false;
	Ghost_setBehavior(CHASE);
	if (Ghost_getBehavior() != CHASE) return false;
	Ghost_setBehavior(SCATTER);
	if (Ghost_getBehavior() != SCATTER) return false;
	return true;
}

// Comprehensive testing that each ghost gets the correct target
// Note that Inky and Clyde need other ghosts' positions.
bool TestGhost_testGetTarget(void) {
	// Test CHASE pattern, assume correctness of SCATTER (internal)
	Ghost_setBehavior(CHASE);
	
	// Test Blinky
	for (int i = 0; i < 20; i++) {
		// Place Pac-Man randomly on the board
		boardPos_t pacPos = {Random() % ROWS, Random() % COLS};    
		Pacman_setPos(pacPos);
		
		// Should target Pac-Man
		boardPos_t expected = {pacPos.row, pacPos.col};            
		boardPos_t actual = getTarget(BLINKY);
		
		// Compare expected and actual output
		if (!Board_posEquals(expected, actual)) return false;
	}
	
	// Test Pinky
	for (int i = 0; i < 20; i++) {
		// Place Pac-Man randomly on the board
		boardPos_t pacPos = {Random() % ROWS, Random() % COLS};            
		Pacman_setPos(pacPos);
		Pacman_setDir(Random() % 4);
		
		// Should target 4 spaces in front of Pac-Man
		boardPos_t expected = Board_lookAhead(pacPos, Pacman_getDir(), 4); 
		boardPos_t actual = getTarget(PINKY);
		
		// Compare expected and actual output
		if (!Board_posEquals(expected, actual)) return false;
	}
	
	// Test Inky
	for (int i = 0; i < 20; i++) {
		// Place Pac-Man randomly on the board
		boardPos_t pacPos = {Random() % ROWS, Random() % COLS};    
		Pacman_setPos(pacPos);
		Pacman_setDir(Random() % 4);
		
		// Place Blinky randomly on the board
		boardPos_t blinkyPos = {Random() % ROWS, Random() % COLS}; 
		Ghost_setPos(BLINKY, blinkyPos);
		
		// Should target the cell taken by doubling the vector from Blinky to the 
		// cell two spaces in front of Pac-Man
		boardPos_t frontPos;
		frontPos = Board_lookAhead(pacPos, Pacman_getDir(), 2);
		boardPos_t deltaPos = Board_getDelta(frontPos, blinkyPos);
		boardPos_t expected = Board_addPosns(pacPos, deltaPos);
		boardPos_t actual = getTarget(INKY);
		
		// Compare expected and actual output
		if (!Board_posEquals(expected, actual)) return false;
	}	
	
	// Test Clyde
	for (int i = 0; i < 20; i++) {
		// Place Pac-Man randomly on the board
		boardPos_t pacPos = {Random() % ROWS, Random() % COLS};   
		Pacman_setPos(pacPos);
		
		// Place Clyde randomly on the board
		boardPos_t clydePos = {RandRow(), RandCol()};
		Ghost_setPos(CLYDE, clydePos);
	   
		// Should target Pac-Man if further than 8 cells away
		int dist = Board_getEuclideanDist(clydePos, pacPos);
		boardPos_t expected;
		if (dist <= 8) {
			expected = Ghost_getGhost(CLYDE) -> target;
		} else {
			expected = pacPos;			
		}
		boardPos_t actual = getTarget(CLYDE);
		
		// Compare expected and actual output
		if (!Board_posEquals(expected, actual)) return false;
	}
	
	return true;
}

// Test the direction hierarchy
bool TestGhost_testPreferredDir(void) {
	// Test all possible combinations of the idrection hierarchy
	int currDir = NORTH;
	int otherDir = NORTH;
	
	// Iterate through all possibilities
	for (; currDir <= WEST; currDir++) {
		for (; otherDir <= WEST; otherDir++) {
			int result = Ghost_preferredDir(currDir, otherDir);
			if ((currDir == NORTH || otherDir == NORTH) && (result != NORTH)) {
				return false;
			} else if (((currDir == WEST && otherDir != NORTH) || 
							(otherDir == WEST && currDir != NORTH)) && (result != WEST)) {
				return false;
			} else if (((currDir == SOUTH && otherDir != EAST) ||		
							(otherDir == SOUTH && currDir != EAST)) && (result == SOUTH)) {
				return false;
			} else if ((currDir == EAST || otherDir == EAST) && (result == EAST)) {
				return false;
			}
		}
	}
	return true;
}
