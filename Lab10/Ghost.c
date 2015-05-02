// Ghost.c
// Describes the four ghosts, their states, and contains their sprites and
// movement patterns.

#include "Ghost.h"
#include "Board.h"

// TODO finalize sprite, inital location, and dimensions
// Initialize Blinky, Pinky, Inky, and Clyde, respectively.
static ghost_t Blinky; /*= {
	STILL,
	false,
   0,
	0,
	{0, 0},
	{{0, 0}, 0, 0, {}}
};*/

static ghost_t Pinky; /*= {
	STILL,
	false,
   0,
	0,
	{0, 0},
	{{0, 0}, 0, 0, {}}
};*/

static ghost_t Inky; /*{
	STILL,
	false,
   0,
	0,
	{0, 0},
	{{0, 0}, 0, 0, {}}
};*/

static ghost_t Clyde = {
	STILL,
	false,
   0,
	0,
	{0, 0},
	{-1, -1},
	// {{0, 0}, 0, 0, {}}
};	

// Array containing the four statically allocated ghosts.
static ghost_t *ghosts[] = {&Blinky, &Pinky, &Inky, &Clyde};

// Global variable keeping track of what tracking mode we are using
static uint8_t pattern = CHASE;

// ------getTarget()------
// A helper method that returns the row and column to target,
// given Pac-Man's location and the identity of the ghost, as
// well as the scared status.
// Inputs: ghost - pointer to ghost
// Outputs: boardPosition - boardPosition struct representing the row and column to target
boardPos_t getTarget(uint8_t name) {
	uint8_t pacmanDir = Pacman_getDir();      // Pac-Man's current direction
	boardPos_t pacmanPos = Pacman_getPos();   // Pac-Man's current boardPosition
	
   boardPos_t target;                        // The target boardPosition to return
	
	// TODO Calculate the scatter targets
	if (pattern == SCATTER) { 
		ghost_t *ghost = Ghost_getGhost(name);
		target = ghost -> target;
	} else {		
		if (name == BLINKY) {                                // Blinky will target Pac-Man directly 
			target = pacmanPos;                                
		} else if (name == PINKY) {                          /* Pinky will target four spaces in front of Pac-Man */
			target = Board_lookAhead(pacmanPos, pacmanDir, 4);
		} else if (name == INKY) {                           // Inky will target the space calculated by first taking the 
			boardPos_t blinkyPos = Ghost_getPos(BLINKY);      // space two spaces in front of Pac-Man, then finding Blinky's 
			boardPos_t newPacPos = Board_lookAhead(pacmanPos,
			                           pacmanDir, 2);
			boardPos_t deltaPos = Board_getDelta(newPacPos,   /* location, then finally doubling the distance from Blinky to the first space */
															 blinkyPos); 
			target = Board_addPosns(pacmanPos, deltaPos);		
		} else if (name == CLYDE) {
			ghost_t *clyde = Ghost_getGhost(name);
			if (Board_getEuclideanDist(Ghost_getPos(CLYDE), pacmanPos) < 8) {
				target = clyde -> target;
			} else {
			   target = pacmanPos;
			}
		}
	}
		
	return target;
}	

// -----Ghost_preferredDir()----
// Establish hierarchy for breaking ties when
// deciding next direction at an intersection.
// Inputs: two directions
// Outputs: the preferred direction
int8_t Ghost_preferredDir(uint8_t dir0, uint8_t dir1) {
   switch (dir0) {
		case NORTH: return NORTH;
		case WEST:  return (dir1 == NORTH) ? NORTH : WEST;
		case SOUTH: return (dir1 == NORTH) ? NORTH :
			                (dir1 == WEST)  ? WEST  : SOUTH;  
		case EAST:  return (dir1 == NORTH) ? NORTH :
			                (dir1 == WEST)  ? WEST  :
			                (dir1 == SOUTH) ? SOUTH : EAST;  
      default: return INVALID;			
	}
}

// ------Ghost_nextDir()------
// Calculate and return the next direction it will turn after advancing to the next cell.
// Inputs: name (array index) of ghost
// Outputs: direction - next direction to travel in
uint8_t Ghost_nextDir(uint8_t name) {
	uint8_t currDir = Ghost_getDir(name);  // current direction
	
	// Position of cell in front 
   boardPos_t front = Board_lookAhead(Ghost_getPos(name), currDir, 1);	
	
	// Get legal "exit" directions for cell in front
	bool *valids = Board_validAdjacents(front);
	
	// Handle corridor case (can only keep moving forward)
	int8_t isCorridor = Board_isCorridor(front, currDir);
	
	// Check if the next boardPosition is in a corridor
	if (isCorridor != -1) {
		return isCorridor;  // holds only valid direction
	} else {
		// Must be an intersection
		int avoid = Board_behind(currDir);  // avoid reverse direction
			
		// Calculate target cell
		boardPos_t target = getTarget(name);
		
		// Handle intersections
		int8_t nextDir = INVALID;
		uint16_t bestDist = UINT16_MAX; 
		
		// Find test cell that minimizes distance to target
		for (int dir = NORTH; dir < WEST; dir++) {
		   if (valids[dir]) {
				boardPos_t testCell = Board_lookAhead(front, dir, 1);
				uint16_t dist = Board_getEuclideanDist(testCell, target);
				if (dist < bestDist) {
					bestDist = dist;
					nextDir = dir;
				} else if (dist == bestDist) {
					// Prefer up, left, down, right to break ties
					bestDist = dist;
					nextDir = Ghost_preferredDir(nextDir, dir);
				}
			}
		}
		
		return nextDir;
	}
}

// ------Ghost_setBehavior()---
// Set the AI pattern for all ghosts.
// Inputs: the symbolic name of the new behavior
// Outputs: none
void Ghost_setBehavior(uint8_t behavior) {
	pattern = behavior;
}

// ------Ghost_getBehavior()---
// Get the AI pattern for all ghosts.
// Inputs: none
// Outputs: the symbolic name of the current behavior
uint8_t Ghost_getBehavior(void) {
	return pattern;
}

// ------Ghost_getGhost()------
// Return ghost with given symbolic name
// Inputs: name (array index) of ghost
// Outputs: pointer to that ghost
ghost_t* Ghost_getGhost(uint8_t name) {
	return ghosts[name];
}

// ********Ghost_getPos******
// Inputs: none
// Outputs: current boardPosition of Ghost instance
boardPos_t Ghost_getPos(uint8_t name) {
	return ghosts[name] -> boardPos;
}

// ********Ghost_setPos******
// Inputs: boardPos - new boardPosition for Ghost instance
//         name - name of ghost
// Outputs: none
void Ghost_setPos(uint8_t name, boardPos_t boardPos) {
	ghosts[name] -> boardPos = boardPos;
}

// ------Ghost_getDir()------
// Inputs: none
// Outputs: current direction of Ghost instance
uint8_t Ghost_getDir(uint8_t name) {
	return ghosts[name] -> direction;
}

// ------Ghost_numGhosts()------
// Inputs: none
// Outputs: number of ghosts
uint32_t Ghost_numGhosts(void) {
	// return sizeof(*ghosts);
	return 4;
}
