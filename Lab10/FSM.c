#include "FSM.h"

// Track the state of the game
state_t state = PLAY;
uint32_t score = 0;
char* message = "";

// ------FSM_getInputs()------
// Read in controller inputs and calculate the next directions
// of relevant objects.
// Inputs: none
// Outputs: none
void FSM_getInputs(void) {
	if (state == MENU) {
		// Read in ADC input
		int32_t buttonInd = ADC_getMenuOption();
		
		// Check if select switch is pressed
		bool isSelect = Switches_getSelect();   // 1 indicates press
		
		// Select and initialize corresponding board
		if (isSelect && (buttonInd != -1)) {
		  // Initialize board
		  // Board_init(buttonInd);
			
			// Transition to PLAY state and reset variables
			score = 0;
			(Pacman_getPacman() -> lives) = LIVES;
			*Board_numDots() = NUM_DOTS;
			state = PLAY;
		}				
	} else if (state == PLAY) {
		// Read in hardware switch input
		int32_t newDir = Switches_getDir();
		
		// Check if valid direction for Pac-Man
		bool isValid = (newDir != -1) && Board_isValid(Pacman_nextPos(newDir));
		
		// Check if Pac-Man's sprite is allowed to turn
		bool canTurn = *Pacman_changedCell(); 
		
		// Set new direction if valid and Pac-Man can turn
		if (isValid & canTurn) {
			Pacman_setDir(newDir);
			*Pacman_changedCell() = false;
		}		
		
		// Process each ghost and update their direction 
		// Ghosts will always be one step "ahead", calculating the direction
		// they'll travel for the next cell while in the current cell.
		for (int i = 0; i < Ghost_numGhosts(); i++) {
			// Pointer to current ghost
			ghost_t *ghost = Ghost_getGhost(i);
			
			// Check if each ghost has reached a new cell
			bool changedCell = ghost -> changedCell; 
			
			// Update direction states on a transition between cells
			if (changedCell) {				
				// Set ghost's direction to "next" direction calculated one cell ago
				ghost -> direction = ghost -> nextDir;
				
				// Calculate and set new "next" direction
			   uint8_t newDir = Ghost_nextDir(i); 
				ghost -> nextDir = newDir;
				ghost -> changedCell = false;   // reset cell transition flag
			}			
		}
	} else if (state == GAME_OVER) {
		// Check if select switch is pressed
		bool isSelect = Switches_getSelect();
		
		// Transition to MENU state if so
		state = (isSelect) ? MENU : GAME_OVER;
	}	
}

// ------FSM_updateBoard()------
// Perform all collision handling and set the new boardPositions
// of relevant objects.
// Inputs: none
// Outputs: none
void FSM_updateBoard(void) {
	if (state == MENU) {
		// Set flag indicating which button is currently highlighted

	} else if (state == PLAY) {
		// Move Pac-Man's sprite and update his board location
		bool changedCell = Sprite_move(Pacman_getSprite(), Pacman_getDir(), Pacman_getSpeed());
		
		if (changedCell) {
			boardPos_t oldPos = Pacman_getPos();
			boardPos_t newPos = Pacman_nextPos(Pacman_getDir());
			Pacman_setPos(newPos);
			
			// Handle any "collisions" between Pac-Man and non-Ghosts
			int8_t state = Board_getCell(Pacman_getPos());
			
			Board_setCell(oldPos, EMPTY);    // Implicitly handle "empty" case
			Board_setCell(newPos, PACMAN);
			
			if (state == FOOD) {             // TODO briefly slow down Pac-Man
				score += FOOD;
				(*Board_numDots())--;
			} else if (state == BIG_FOOD) {  // TODO handle energizers 
				score += BIG_FOOD;	
				Ghost_setBehavior(SCARED);    // TODO need a timer to time when to stop
			} 
			
			*Pacman_changedCell() = true;
		}
		
		// Move each Ghost and update their board locations (only internally)
		for (int i = 0; i < Ghost_numGhosts(); i++) {
		   ghost_t *ghost = Ghost_getGhost(i);
			// bool changedCell = Sprite_move(&(ghost -> sprite), ghost -> direction, ghost -> speed);
			// TODO fix where sprite is held
			
			if (changedCell) {
				// Handle any collisions between Pac-Man and ghosts
				int8_t state = Board_getCell(ghost -> boardPos);
				
				// Pac-Man loses (TODO handle energizers)
				if (state == PACMAN) {
					(Pacman_getPacman() -> lives)--;
					if ((Pacman_getPacman() -> lives) <= 0) {
						message = "YOU LOSE!";
						state = GAME_OVER;
					}
					// Pacman_setPos();     TODO Pac-Man's home square?
				}
				
				ghost -> changedCell = true;  
			}		
		}		
		
		// Check for winning condition (all dots consumed)
		if (*Board_numDots() <= 0) {
			message = "YOU WIN!";      // TODO leveling system
			state = GAME_OVER;
		}		
	} else if (state == GAME_OVER) {
		// Do nothing
		return;
	}	
}

// ------FSM_updateScreen()------
// Draw the new state of the board to the screen.
// Inputs: none
// Outputs: none
void FSM_updateScreen(void) {
	if (state == MENU) {
		// Draw entire menu on first tick of state
		
		// Redraw highlighted button/cursor
		
	} else if (state == PLAY) {
		// Draw entire board on first tick of state
		
		// Redraw any elements that have changed or moved
		
	} else if (state == GAME_OVER) {
		// Draw game over screen (currently static)
		
	}	
}
