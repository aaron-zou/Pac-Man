#ifndef __Sprite_h__
#define __Sprite_h__

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Describes the row-column boardPosition of an object.  
typedef struct boardPos {
   int16_t row, col; 
} boardPos_t; 

// Describes a pixel center of an object
typedef struct screenPos {
	int16_t x, y;            // pixels are fixed point and in units 0.01 pixels
} screenPos_t;

// Describe a generic sprite.
typedef struct sprite {
	screenPos_t screenPos;
	int16_t width, height;
	const uint16_t image[];
} sprite_t;

// ------Sprite_move()------
// Move a sprite towards the current target, given its speed.
// Note: pixels are fixed point with units (0.01 pixels)
// Input: sprite - pointer to a sprite
//        dir - direction the sprite is facing
//        speed - number of pixels to move per second
// Output: true if the move resulted in the boardPos changing
//         false otherwise
bool Sprite_move(sprite_t *sprite, uint8_t dir, uint16_t speed);

#endif /* Sprite.h */
