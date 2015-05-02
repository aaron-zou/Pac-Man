// TestHarness.c
// Runs the tests for each file, noting how many pass and which ones fail

#include "TestHarness.h"
#include "TestPacman.h"
#include "TestGhost.h"
#include "TestBoard.h"
#include "TestFSM.h"
#include "TestSprite.h"
#include "TestInput.h"
#include "TestGraphics.h"
#include "tm4c123gh6pm.h"

bool runAllTests(void) {
	UART_Init();
	Random_Init(NVIC_ST_CURRENT_R);
	
	bool p = runPacmanTests();
   bool g = runGhostTests();
   bool b = runBoardTests();
   bool r = runGraphicsTests();
   bool f = runFSMTests();
   bool s = runSpriteTests();
   bool i = runInputTests();
	
	return p && g && b && r && f && s && i;  
}

bool runPacmanTests(void) {
	return true;
}

bool runGhostTests(void) {
	bool results[4];
	
	results[0] = TestGhost_testSetBehavior();
	results[1] = TestGhost_testGetTarget();
	results[2] = TestGhost_testPreferredDir();
	results[3] = TestGhost_testNextDir();
	
	for (int i = 0; i < sizeof(results); i++) 
		if (!results[i]) return false;
	return true;
}

bool runBoardTests(void) {
	bool results[9];
	
	results[0] = TestBoard_testSetCell();       
	results[1] = TestBoard_testSetBoard();      
	results[2] = TestBoard_testValid();         
	results[3] = TestBoard_testBehind();        
	results[4] = TestBoard_testEuclidDist();    
	results[5] = TestBoard_testManhattanDist(); 
	results[6] = TestBoard_testAdjacents();     
	results[7] = TestBoard_testIsCorridor();    
	results[8] = TestBoard_testLookahead();     
	
	for (int i = 0; i < 9; i++) 
		if (!results[i]) return false;
	return true;
}

bool runGraphicsTests(void) {
	return true;
}

bool runFSMTests(void) {
	return true;
}

bool runSpriteTests(void) {
	return true;
}

bool runInputTests(void) {
	return true;
}
