#ifndef __FSM_h__
#define __FSM_h__

#include "ADC.h"
#include "Switches.h"
#include "Board.h"
#include "Graphics.h"

typedef enum {MENU, PLAY, GAME_OVER} state_t;

// ------FSM_getInputs()------
// Read in controller inputs and calculate the next directions
// of relevant objects.
// Inputs: none
// Outputs: none
void FSM_getInputs(void);

// ------FSM_updateBoard()------
// Perform all collision handling and set the new boardPositions
// of relevant objects.
// Inputs: none
// Outputs: none
void FSM_updateBoard(void);

// ------FSM_updateScreen()------
// Draw the new state of the board to the screen.
// Inputs: none
// Outputs: none
void FSM_updateScreen(void);

#endif
