// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "TExaS.h"
#include "Sound.h"
#include "FSM.h"
#include "TestHarness.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

// Global variables
bool debugMode = true;
bool runGame = true;   // use to synchronize animation loop
uint8_t frame = 0;        // track current frame
uint8_t fps = 30;

// You can use this timer only if you learn how it works
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time--) {}
    count--;
  }
}

// ***SysTick_Init()*********
// Initialize SysTick periodic interrupts with
// a period of 1/n seconds, where n is the frames per second.
// Input: fps - frame rate (frames per second)
// Outputs: none
void SysTick_Init(int fps) {
  int reload = (80000000 / fps) - 1;                // calculate reload value
  NVIC_ST_CTRL_R = 0;                               // disable SysTick during setup
  NVIC_ST_RELOAD_R = reload;                         // reload value
  NVIC_ST_CURRENT_R = 0;                            // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF)  // priority 0
                     | 0x00000000; 
  NVIC_ST_CTRL_R = 0x07;                            // enable SysTick with core clock and interrupts   
}

// ***SysTick_Handler()*****
// Control the animation cycle by synchronizing 
// loop execution with SysTick's reload value
// Input: none
// Output: none
void SysTick_Handler(void) {
   runGame = true;   
   frame = (frame + 1) % fps;
}

// Implements the animation cycle.
int main(void){
   PLL_Init();                     // 80 MHz bus frequency
   SysTick_Init(fps);              // Initialize game's frame rate
   // ST7735_InitR(INITR_REDTAB);     // Initialize LCD
   // ADC_Init();
   Switches_Init();
   Sound_Init();
   Board_init(0);
   
   // Run test cases
   if (debugMode) {
      if (runAllTests()) {
         return 0;
      } else {
         char* message = "Didn't pass all tests...";
         UART_OutString(message);
         return -1;
      }      
   }
   
   // Animation loop
   while (true) {
      if (runGame) {
         FSM_getInputs();      // Read controller input and set new directions for moving objects
         FSM_updateBoard();    // Handle collisions and set new boardPositions for moving objects
         FSM_updateScreen();   // Draw the new state of the board
         runGame = false;
      }         
   }
}



