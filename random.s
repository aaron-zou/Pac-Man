; Random number generator;
; Linear congruential generator 
; from Numerical Recipes by Press et al.
; Jonathan Valvano

; How to use: 
; 1) call Random_Init once with a seed
;     Random_Init(1);
;     Random_Init(NVIC_CURRENT_R);
; 2) call Random over and over to get a new random number
;   n = Random();    // 32 bit number
;   m = (Random()>>24)%60; // a number from 0 to 59

       THUMB
       AREA    DATA, ALIGN=2
M      SPACE   4
ROWS   EQU     36             ; double check these numbers
COLS   EQU     18        
       ALIGN          
       AREA    |.text|, CODE, READONLY, ALIGN=2
	  
       EXPORT  Random_Init
       EXPORT  Random
       EXPORT  Random32
	   EXPORT  RandRow
	   EXPORT  RandCol
	
Random_Init
       LDR R2,=M       ; R4 = &M, R4 points to M
       MOV R0,#1       ; Initial seed
       STR R0,[R2]     ; M=1
       BX  LR
;------------Random32------------
; Return R0= random number
; Linear congruential generator 
; from Numerical Recipes by Press et al.
Random32 LDR R2,=M    ; R2 = &M, R4 points to M
       LDR R0,[R2]  ; R0=M
       LDR R1,=1664525
       MUL R0,R0,R1 ; R0 = 1664525*M
       LDR R1,=1013904223
       ADD R0,R1    ; 1664525*M+1013904223 
       STR R0,[R2]  ; store M
       BX  LR

;------------Random------------
; Return R0= random number, 0 to 255
; Linear congruential generator 
; from Numerical Recipes by Press et al.
Random LDR R2,=M     ; R2 = &M, R4 points to M
       LDR R0,[R2]   ; R0=M
       LDR R1,=1664525
       MUL R0,R0,R1  ; R0 = 1664525*M
       LDR R1,=1013904223
       ADD R0,R1     ; 1664525*M+1013904223 
       STR R0,[R2]   ; store M
       LSR R0,R0,#24 ; top 8 bits of number
       BX  LR
	   
;------------RandRow------------
; Return R0= random number, 0 to ROWS - 1
; Linear congruential generator 
; from Numerical Recipes by Press et al.
RandRow LDR R2,=M     ; R2 = &M, R4 points to M
       LDR  R0,[R2]   ; R0=M
       LDR  R1,=1664525
       MUL  R0,R0,R1  ; R0 = 1664525*M
       LDR  R1,=1013904223
       ADD  R0,R1     ; 1664525*M+1013904223 
       STR  R0,[R2]   ; store M
       LSR  R0,R0,#24 ; top 8 bits of 
	   LDR  R2,=ROWS
	   LDR  R2,[R2]
	   UDIV R1,R0,R2  ; Random32 / ROWS
	   MUL  R1,R1,R2  ; get integer part
	   SUB  R0,R0,R1  ; store Random32 % ROWS
       BX   LR

;------------RandCol------------
; Return R0= random number, 0 to COLS - 1
; Linear congruential generator 
; from Numerical Recipes by Press et al.
RandCol LDR R2,=M     ; R2 = &M, R4 points to M
       LDR  R0,[R2]   ; R0=M
       LDR  R1,=1664525
       MUL  R0,R0,R1  ; R0 = 1664525*M
       LDR  R1,=1013904223
       ADD  R0,R1     ; 1664525*M+1013904223 
       STR  R0,[R2]   ; store M
       LSR  R0,R0,#24 ; top 8 bits of number
	   LDR  R2,=COLS  
	   LDR  R2,[R2]
	   UDIV R1,R0,R2  ; Random32 / ROWS
	   MUL  R1,R1,R2  ; get integer part
	   SUB  R0,R0,R1  ; store Random32 % ROWS
       BX   LR
		
       ALIGN      
       END  
           