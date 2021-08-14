;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


; 160036526 - Matheus Elyasha Lopes
;;;;
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
PROVA_A0: 	MOV 	#12,R4 ; Dividendo
			MOV 	#5,R5 ; Divisor
			MOV		#0, R6 ; Quociente
			MOV		#0, R7 ; Resto

			JMP 	algoritmo ;
			NOP
;
; Sub-rotina
; Recebe: R4 = Dividendo
; R5 = Divisor
; Retorna: R6 = Quociente
; R7 = Resto

algoritmo:	cmp R5, R4	; Comparar R4 com R5
			jz	iguais	; Se forem iguais
			jge	maior	; Se R4 for maior que R5
			jmp menor	; Se R4 for menor que R5

iguais:
			MOV #1, R6  ; Quociente 1
			MOV #0, R7  ; Resto 0
			jmp fim		; Fim do programa


maior:
			inc R6		; Incrementar o R6 de 1
			sub	R5, R4	; Subtrair dividor do dividendo
			cmp R5, R4  ;
			jz  maior_iguais  ;
			cmp R5, R4  ;
			jge maior_maior   ;
			mov R4, R7  ; O resto é o dividendo
			jmp fim     ;

maior_iguais:
			inc R6
			mov #0, R7  ; O resto é zero
			jmp fim

maior_maior:
			inc R6
			sub R5, R4
			cmp R5, R4
			jz maior_iguais
			cmp R5, R4
			jge maior_maior
			mov R4, R7 ; O resto é o dividendo
			jmp fim


menor:
			mov R4, R7  ; Resto é o dividendo
			mov #0, R6	; Quociente 0
			jmp fim		; Fim do programa

fim:
			ret


                                            

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
