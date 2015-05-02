// Pacman.c
// Describes the player (Pac-Man), his state, and the methods that operate on him.
// There is only one Pac-Man instance, which is created at compile-time.
// Provide ways for tracking the sprite.

#include "Pacman.h"
#include "Board.h"

// TODO finalize sprite, inital location, and dimensions
// Singleton instance of Pac-Man
static pacman_t Pacman; /* = {
   LIVES,                   // lives
   WEST,                // direction
   false
   80,                  // speed (pixels/sec)
   0,                   // time spent in cell
   {0, 0, 0, 0, {}}   // sprite
}; */

// Pointer to Pac-Man instance.
pacman_t *pacPt = &Pacman;

// ------Pacman_nextPos()------
// Returns the next boardPosition Pac-Man would occupy with
// the given direction.
// Inputs: direction - hypothetical direction
// Outputs: boardPos - next boardPosition
boardPos_t Pacman_nextPos(uint32_t direction) {
   return Board_lookAhead(Pacman_getPos(), direction, 1);
}

// ------Pacman_setPos()------
// Solidifies Pac-Man's new boardPosition.
// Inputs: boardPos - new boardPosition to set
// Outputs: none
void Pacman_setPos(boardPos_t boardPos) {   
   pacPt -> boardPos = boardPos;
}

// ------Pacman_changedCell()------
// Inputs: none
// Outputs: pointer to whether Pacman just changed cells
bool* Pacman_changedCell(void) {
   return &(pacPt -> changedCell);
}

// ------Pacman_getPos()------
// Inputs: none
// Outputs: current boardPosition of Pac-Man instance
boardPos_t Pacman_getPos(void) {
   return pacPt -> boardPos;
}

// ------Pacman_setDir()------
// Inputs: boardPos - new direction to face
// Outputs: none
void Pacman_setDir(uint8_t direction) {
   pacPt -> direction = direction;
}

// ------Pacman_getDir()------
// Inputs: none
// Outputs: direction - current direction 
uint8_t Pacman_getDir(void) {
   return pacPt -> direction;
}

// ------Pacman_getPacman()------
// Return a pointer to Pac-Man for modifying or getting state.
// Inputs: none
// Outputs: pacPt - pointer to Pac-Man instance
pacman_t *Pacman_getPacman(void) {
   return pacPt;
}

// ------Pacman_getSpeed()------
// Return Pac-Man's speed.
// Inputs: none
// Outputs: speed - Pac-Man's speed
uint16_t Pacman_getSpeed(void) {
   return pacPt -> speed;
}

// ------Pacman_getSprite()------
// Return a pointer to Pac-Man's sprite for modifying or getting state.
// Inputs: none
// Outputs: sprite - pointer to Pac-Man's sprite
sprite_t *Pacman_getSprite(void) {
   sprite_t *sprite = &(pacPt -> sprite);
   return sprite;
}
