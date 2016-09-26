// Switches.c 
// Initialize & read inputs from PORTE 
// arrow key switches 
// select/pause swtich 
// hardware connections: PE0=N, PE1=E, PE3=S, PE5=W, PE4=Select/Pause

#include "Switches.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

uint32_t SwitchIn;

// *********Switches_Init()*******************
// Initialize boardPostive logic switches on PE0,1,3,4,5
// "arrow keys" for the game (0,1,3,5) and select/pause button (4)
// Input: none
// Output: none
void Switches_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x10;     				// Turn on clock for Port E
	delay = SYSCTL_RCGC2_R;					   // wait 2 bus cycyles 

	GPIO_PORTE_AMSEL_R &= ~0x3B;				// Disable analog function on PE5,4,3,1,0
  GPIO_PORTE_PCTL_R &= ~0x000FFF0FF; 	 // Enable regular GPIO 5,4,3,1,0
  GPIO_PORTE_DIR_R &= ~0x3B;  				// Inputs on PE5,4,3,1,0
  GPIO_PORTE_AFSEL_R &= ~0x3B; 				// Regular function on PE5,4,3,1,0
  GPIO_PORTE_DEN_R |= 0x3B;   				// Enable digital on PE5,4,3,1,0
	
}

// *********Switches_getDir()*******************
// Reads PORTE Data Register to get inputs 
// converts them to 0,1,2,3 which coorespond to cardinal diretions: N,E,S,W
// Input: none
// Output: 0,1,2,3 or -1
int Switches_getDir(void){
	SwitchIn = GPIO_PORTE_DATA_R;				// Read Port E 
	SwitchIn &= 0x2B;
	if(SwitchIn == 0x01){						// North key pressed
		return 0;
	} else if(SwitchIn == 0x02){				// East key pressed
		return 1;
	} else if(SwitchIn == 0x20){				// South key pressed
		return 2; 
	} else if (SwitchIn == 0x08){				// West key pressed 
		return 3;
	} else {
		return -1;
	}		
}

// *********Switches_getPause()*******************
// Reads PORTE Data Register to see if pause switch is pressed
// Input: none
// Output: false for not pressed, true for pressed
bool Switches_getPause(void){
	SwitchIn = GPIO_PORTE_DATA_R; 			//Read from Port E
	SwitchIn &= 0x10;
	if(SwitchIn == 0x10) {
		return 1; 												//pause pressed
	} else 
		return 0; 												//pause not pressed 
}


// *********Switches_getSelect()*******************
// Reads PORTE Data Register to see if select switch is pressed
// Input: none
// Output: false for not pressed, true for pressed
bool Switches_getSelect(void){
	SwitchIn = GPIO_PORTE_DATA_R; 			//Read from Port E
	SwitchIn &= 0x10;										
	if(SwitchIn == 0x10) {
		return 1; 												//select pressed
	} else 
		return 0; 												//select not pressed
	
}

