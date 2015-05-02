#ifndef __TestHarness_h__
#define __TestHarness_h__

#include "FSM.h"
#include "assert.h"
#include "Constants.h"
#include "UART.h"

// Run all test cases
bool runAllTests(void);

// Run test cases for an individual file
bool runPacmanTests(void);
bool runGhostTests(void);
bool runBoardTests(void);
bool runGraphicsTests(void);
bool runFSMTests(void);
bool runSpriteTests(void);
bool runInputTests(void);

#endif /* TestHarness.h */
