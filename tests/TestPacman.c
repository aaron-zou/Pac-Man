// Series of unit tests for Pacman.c

#include "TestPacman.h"

static pacman_t *pt;

bool testPacmanInit(void) {
	pt = Pacman_getPacman();
	pt -> lives = 3;
	pt -> direction = NORTH;
	pt -> changedCell = false;
	pt -> speed = 5;
	if (pt -> lives != 3)           return false;
	if (pt -> direction != NORTH)   return false;
	if (pt -> changedCell != false) return false;
	if (pt -> speed != 5)           return false;
	return true;
}

bool testPacmanNextPos(void) {
	pt = Pacman_getPacman();
	boardPos_t pos = {0, 0};
	Pacman_setPos(pos);
	pt -> direction = EAST;
	return Pacman_nextPos(pt -> direction).row == 0 &&
			 Pacman_nextPos(pt -> direction).col == 1;
}

bool testPacmanSetPos(void) {
	pt = Pacman_getPacman();
	boardPos_t pos = {4, 4};
	Pacman_setPos(pos);
	return Pacman_getPos().row == 4 && Pacman_getPos().col == 4;
}

bool testPacmanSprite(void) {
	return true;
}
