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


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
; 160036526 - Matheus Elyasha Lopes
;;;;
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

; Sub-rotina PROVA_A1
; Recebe: R4 = Endereço de uma equação;
; Retorna: R5 = Resultado da equação

;Recursos a serem usados pela sub-rotina:
	;R4 = Ponteiro, Endereço de uma equação;
	;R5 = Resultado da equação
	;R6 = registrador auxiliar para salvar o byte
	;R7 = Número de digitos

			MOV #vetor, R4

PROVA_A1:
			clr R5   ; Limpar o resultado
			mov #0, R7 ; Número de digitos


loop:			mov.b 	@R4, R6 ; Pegar o byte

		    	cmp.b 	#0x2B, R6 ; Comparar se é soma
			jz	somar

			cmp.b #0x3D, R6 ; 0x3D é =, 0x2B é +
			jz somar


			; O character é um número
			inc.b R7 ; Aumenta o número de dígitos
			inc R4  ; Incrementa o ponteiro
			jmp loop


somar:
			; Somar usando potência de 10 e o número de digitos para as potências

			mov.b -1(R4), R8 ; Digito das unidades
			mov.b -2(R4), R9 ; Digito das dezenas
			mov.b -3(R4), R10 ; Digito das centenas de milhar

			cmp #1,R7
			jlo proximo_numero
			; Multiplicar por 1
			; Antes de somar/multiplicar precisa converter de ASCII para um valor Hexadecimal
			sub #0x30, R8
			add R8, R5

			; Multiplicar por 10
			cmp #2, R7
			jlo proximo_numero
			; Antes de somar/multiplicar precisa converter de ASCII para um valor Hexadecimal
			sub #0x30, R9
			mov #10, &MPY
			mov R9, &OP2
			add &RES0, R5

			; Multiplicar por 100
			cmp #3, R7
			jlo proximo_numero
			; Antes de somar/multiplicar precisa converter de ASCII para um valor Hexadecimal
			sub #0x30, R10
			mov #100, &MPY
			mov R10, &OP2
			add &RES0, R5

			; Multiplicar por 1000
			cmp #4, R7
			jlo proximo_numero
			; Antes de somar/multiplicar precisa converter de ASCII para um valor Hexadecimal
			sub #0x30, R11
			mov #1000, &MPY
			mov R11, &OP2
			add &RES0, R5

			; Multiplicar por 10000
			cmp #5, R7
			jlo proximo_numero
			; Antes de somar/multiplicar precisa converter de ASCII para um valor Hexadecimal
			sub #0x30, R12
			mov #10000, &MPY
			mov R12, &OP2
			add &RES0, R5

			jmp proximo_numero

proximo_numero:
			cmp.b #0x3D, R6 ; 0x3D é =, 0x2B é +
			jz fim
			clr R7
			inc R4
			jmp loop

fim:		ret
			nop

			.data
vetor:		.byte	"729+1041+321+45="
                                            

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
            
