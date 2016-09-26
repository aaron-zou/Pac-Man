#ifndef __Pacman_h__
#define __Pacman_h__

#include <stdint.h>

#include "Sprite.h"
#include "Constants.h"

// Describes the Pac-Man player.
typedef struct pacman {
	uint8_t lives;
	uint8_t direction;
	bool changedCell;
	uint16_t speed;            // the pixels moved per tick
	boardPos_t boardPos;
	sprite_t sprite;
} pacman_t; 


// ------Pacman_nextPos()------
// Returns the next boardPosition Pac-Man would occupy with
// the given direction.
// Inputs: direction - a direction
// Outputs: boardPos - next boardPosition
boardPos_t Pacman_nextPos(uint32_t direction);

// ------Pacman_setPos()------
// Solidifies Pac-Man's new boardPosition.
// Inputs: boardPos - new boardPosition to set
// Outputs: none
void Pacman_setPos(boardPos_t boardPos);

// ------Pacman_changedCell()------
// Inputs: none
// Outputs: pointer to whether Pacman just changed cells
bool* Pacman_changedCell(void);

// ------Pacman_getPos()------
// Inputs: none
// Outputs: current boardPosition of Pac-Man instance
boardPos_t Pacman_getPos(void);
	
// ------Pacman_setDir()------
// Inputs: pdirection - new direction to face
// Outputs: none
void Pacman_setDir(uint8_t direction);

// ------Pacman_getDir()------
// Inputs: none
// Outputs: direction - current direction 
uint8_t Pacman_getDir(void);

// ------Pacman_getPacman()------
// Return a pointer to Pac-Man for modifying or getting state.
// Inputs: none
// Outputs: pacPt - pointer to Pac-Man instance
pacman_t *Pacman_getPacman(void);

// ------Pacman_getSpeed()------
// Return Pac-Man's speed.
// Inputs: none
// Outputs: speed - Pac-Man's speed
uint16_t Pacman_getSpeed(void);

// ------Pacman_getSprite()------
// Return a pointer to Pac-Man's sprite for modifying or getting state.
// Inputs: none
// Outputs: sprite - pointer to Pac-Man's sprite
sprite_t *Pacman_getSprite(void);

#endif
