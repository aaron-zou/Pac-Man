// Sprite.c
// Handle movement of a sprite for Pac-Man

#include "Sprite.h"
#include "Constants.h"

// ------Sprite_getMoveTarget()------
// Returns the (x, y) target, given the faced direction.
// Inputs: dir - faced direction
// Outputs: pos - offset position of median line
screenPos_t Sprite_getMoveTarget(uint8_t dir) {
	screenPos_t target = {-1, -1};
	switch (dir) {
		case NORTH: target.x = 0; target.y = CELL_SIZE / 2; break;
		case EAST:  target.x = CELL_SIZE / 2; target.y = 0; break;
		case SOUTH: target.x = 0; target.y = CELL_SIZE / 2; break;
		case WEST:  target.x = -CELL_SIZE / 2; target.y = 0; break;
	}
	return target;
}

// ------Sprite_move()------
// Move a sprite towards the current target, given its speed.
// Note: pixels are fixed point with units (0.01 pixels)
// Input: sprite - pointer to a sprite
//        dir - direction the sprite is facing
//        speed - number of pixels to move per second
// Output: true if the move resulted in the boardPos changing
//         false otherwise
bool Sprite_move(sprite_t *sprite, uint8_t dir, uint16_t speed) {
   // Calculate vector to apply
	screenPos_t target = Sprite_getMoveTarget(dir);
	int magnitude = sqrt((target.x * target.x) + (target.y * target.y));   	
   int16_t deltax = (target.x * speed) / magnitude; 
   int16_t deltay = (target.y * speed) / magnitude;

   // Add deltas and check for a cell change
	int threshold = CELL_SIZE / 2;
	bool changed = true;
	
	int16_t newX = sprite -> screenPos.x + deltax;
	int16_t newY = sprite -> screenPos.y + deltay;
	
	// Check edge conditions
	if (newX < -threshold) {		
		newX = threshold + deltax;
	} else if (newX > threshold) {
		newX = -threshold + deltax;			
	} else if (newY < -threshold) {	
		newY = threshold + deltay;
	} else if (newY > threshold) {
		newY = -threshold + deltay;		
	} else {
		changed = false;
	}
	
	sprite -> screenPos.x = newX;
	sprite -> screenPos.y = newY;
	
	return changed;
}
