#ifndef __ADC_h__
#define __ADC_h__

#include <stdint.h>

//------------ADC_Init----------
// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void);

//---------ADC_getMenuOption------
// Busy-wait analog to digital conversion,
// then return which menu button is being selected.
// Input: none
// Output: 1,2,3, depeding on whats selected or -1 if invalid 
//the index of the button currently selected,
//         or -1 if no valid button is selected
int32_t ADC_getMenuOption(void);

//---------ADC_Convert------------
//converet input from 4095 to pixel number
//input: int data from 0-4095
//output:int cooresponding to pixel number in y direction (0-159) 

uint32_t ADC_Convert(uint32_t input);


#endif 
