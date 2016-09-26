#ifndef __TestGhost_h__
#define __TestGhost_h__

#include "Ghost.h"
#include "Board.h"

extern int8_t Ghost_preferredDir(uint8_t dir0, uint8_t dir1);
extern boardPos_t getTarget(uint8_t name);

bool TestGhost_testNextDir(void);
bool TestGhost_testSetBehavior(void);
bool TestGhost_testGetTarget(void);
bool TestGhost_testPreferredDir(void);

#endif /* TestGhost.h */

