#ifndef __Ghost_h__
#define __Ghost_h__

#include "Pacman.h"
#include "Sprite.h"
#include "Constants.h"

// Define template for ghost.
typedef struct ghost {
	uint8_t direction;
	uint8_t nextDir;
	bool changedCell;
	uint16_t speed;
	boardPos_t boardPos;
	boardPos_t target;     // fixed target used when the ghosts are in "scatter" mode
	// sprite_t sprite;
} ghost_t;


// ------Ghost_nextDir()------
// Calculate and return the next direction to travel in, based on a number of factors, 
// including which ghost, whether the ghost is panicked, and the values of the nearby cells.
// Inputs: name (array index) of ghost
// Outputs: direction - next direction to travel in
uint8_t Ghost_nextDir(uint8_t name);

// ------Ghost_setBehavior()---
// Set the AI pattern for all ghosts.
// Inputs: the symbolic name of the new behavior
// Outputs: none
void Ghost_setBehavior(uint8_t behavior);

// ------Ghost_getBehavior()---
// Get the AI pattern for all ghosts.
// Inputs: none
// Outputs: the symbolic name of the current behavior
uint8_t Ghost_getBehavior(void);

// ------Ghost_getGhost()------
// Return ghost with given symbolic name
// Inputs: name (array index) of ghost
// Outputs: pointer to that ghost
ghost_t* Ghost_getGhost(uint8_t name);

// ------Ghost_getPos()------
// Inputs: none
// Outputs: current boardPosition of Ghost instance
boardPos_t Ghost_getPos(uint8_t name);

// ------Ghost_setPos()------
// Inputs: boardPos - new boardPosition for Ghost instance
//         name - name of ghost
// Outputs: none
void Ghost_setPos(uint8_t name, boardPos_t boardPos);

// ------Ghost_getDir()------
// Inputs: none
// Outputs: current direction of Ghost instance
uint8_t Ghost_getDir(uint8_t name);

// ------Ghost_numGhosts()------
// Inputs: none
// Outputs: number of ghosts
uint32_t Ghost_numGhosts(void);

#endif
