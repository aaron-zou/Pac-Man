// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0

#include "ADC.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

#define option1start        12         //begining of option1 box 
#define option1end          45         //end of option1 box
#define option2start        50         //begining of option2 box
#define option2end          95         //end of option2 box
#define option3start       100         //begining of option3 box 
#define option3end         145         //end of option 3 box 

//------------ADC_Init----------
// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void) {
  volatile unsigned long delay;

  SYSCTL_RCGCGPIO_R |= 0x11;      // 1) activate clock for Port E 
  while((SYSCTL_PRGPIO_R&0x10) == 0){};
  GPIO_PORTE_DIR_R &= ~0x4;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate fun on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog fun on PE2
  SYSCTL_RCGCADC_R |= 0x01;       // 6) activate ADC0 
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;
  ADC0_PC_R = 0x01;               // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Seq 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1;  // 11) Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         
  ADC0_SAC_R=6;                              // take average to stabilize 
   
  return;
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
   uint32_t data;
   ADC0_PSSI_R = 0x0008;            
   while((ADC0_RIS_R&0x08)==0){};   
   data = ADC0_SSFIFO3_R&0xFFF; 
   ADC0_ISC_R = 0x0008; 
   return data;  
}


//---------ADC_getMenuOption------
// Busy-wait analog to digital conversion,
// then return which menu button is being selected.
// Input: none
// Output: 1,2,3, depeding on whats selected or -1 if invalid 

//the index of the button currently selected,
//         or -1 if no valid button is selected
int32_t ADC_getMenuOption(void) {
   uint32_t Data;                      //variable to store data from slide pot in (0-4095)
   Data= ADC_In(); 
   Data = ADC_Convert(Data); 
   if(Data < option1start){
      return -1;
   } else if(Data < option1end){
      return 1;
   } else if(Data < option2start){
      return -1; 
   } else if(Data < option2end){
      return 2;
   } else if(Data < option3start){
      return -1; 
   } else if(Data < option3end){
      return 3; 
   } else 
      return -1; 
}


//---------ADC_Convert------------
//converet input from 4095 to pixel number
//input: int data from 0-4095
//output:int cooresponding to pixel number in y direction (0-159) 

uint32_t ADC_Convert(uint32_t input){
   uint32_t Pixel;                   //pixel number that cooresponds to menu option (0-160)
   Pixel = (160*input + 2048)/4096; 
   return Pixel;
}


