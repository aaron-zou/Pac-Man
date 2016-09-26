; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
		
curdig		EQU		0x0

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec

		PUSH{R0,LR}	
		PUSH{R4-R11}	
	
		;SUB		SP, #4
		;MOV		R5,#0x0
		;STR		R5,[SP,#0]
		MOV		R6,#0
		MOV		R7,#10
		
mod		UDIV	R10,R0,R7			;modulus 
		MUL		R11,R10,R7
		SUB		R4,R0,R11
		
		ADD		R4,R4,#0x30			;convert to ascii
		SUB		SP,#4 				;allocate space for next value
		STR		R4,[SP, #0]			;store the least significant dig into the allocated area for our array 
		UDIV	R0,R0,R7
		ADD		R6,R6,#0x1 ;increment count 
		CMP		R0,#0x0
		BNE		mod
		
nextC	LDR		R0,[SP]				;loop through using the values on the stack and print them to screeen and deallocate the space 
		BL		ST7735_OutChar
		ADD		SP, #4
		SUB		R6, #1 
		CMP		R6, #0	
		BNE		nextC
		
		
		;ADD	SP,#4
		POP{R4-R11}
		POP{R0,PC}
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
		PUSH{R0,LR}
		PUSH{R4-R11}
		;SUB		SP, #4
		;MOV		R5,#0x0
		;STR		R5,[SP,#0]
		MOV		R7,#10
		MOV		R6,#3
		MOV		R8,#0x2E
		MOV		R9,#10000
		MOV 	R5, #4
		
		CMP R0, R9
		
		BGE bignumber		;branch if number is greater than 10000
		;BVS	bignumber
		;BPL	bignumber
		CMP	R0,#0xFFFFFFFF
		BEQ bignumber
mod2	UDIV	R10,R0,R7
		MUL		R11,R10,R7
		SUB		R4,R0,R11
		
		ADD		R4,R4,#0x30
		SUB		SP,#4
		STR		R4,[SP, #curdig]
		UDIV	R0,R0,R7 
		SUB		R6,R6,#1
		CMP		R6,#0x0
		BNE		mod2
		
		SUB		SP, #4
		STR		R8,[SP,#curdig]
		
		UDIV	R10,R0,R7
		MUL		R11,R10,R7
		SUB		R4,R0,R11
		
		ADD		R4,R4,#0x30
		SUB		SP,#4
		STR		R4,[SP, #curdig]
		
		MOV		R6,#5
nextC1	LDR		R0,[SP]
		BL		ST7735_OutChar
		ADD		SP, #4
		SUB		R6, #1 
		CMP		R6, #0	
		BNE		nextC1
		
		;ADD		SP,#24
		B   done
		
bignumber
		MOV R6, #3				;output stars
loopst	MOV R4, #0x2A
		SUB SP,#4
		STR R4, [SP, #curdig]
		SUB R6, R6, #1
		CMP R6, #0x0
		BNE loopst
		 
		SUB SP, #4
		STR R8, [SP, #curdig]
		
		SUB SP, #4
		STR R4, [SP, #curdig]
		
		MOV		R6,#5
nextC2	LDR		R0,[SP]
		BL		ST7735_OutChar
		ADD		SP, #4
		SUB		R6, #1 
		CMP		R6, #0	
		BNE		nextC2
done
		POP{R4-R11}
		POP{R0,PC}
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
