#ifndef __Switches_h__
#define __Switches_h__

#include "Constants.h"

// ------Switches_Init()------
// Initialize boardPostive logic switches on PE0-3 
// "arrow keys" for the game 
// Input: none
// Output: none
void Switches_Init(void);

// ------Switches_getDir()------
// Reads PORTE Data Register to get inputs 
// converts them to 0,1,2,3 which coorespond to cardinal diretions: N,E,S,W
// Input: none
// Output: North, East, South, West, -1 if none or multiple switches are pressed
int Switches_getDir(void);

// ------Switches_getPause()------
// Reads PORTE Data Register to see if pause switch is pressed
// Input: none
// Output: false for not pressed, true for pressed
bool Switches_getPause(void);

// ------Switches_getSelect()------
// Reads PORTE Data Register to see if select switch is pressed
// Input: none
// Output: false for not pressed, true for pressed
bool Switches_getSelect(void);

























#endif
