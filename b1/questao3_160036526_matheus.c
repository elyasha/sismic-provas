#include <msp430.h>

#define LED1_ON (P1OUT |= BIT0)
#define LED1_OFF (P1OUT &= ~BIT0)
#define LED1_TOGGLE (P1OUT ^= BIT0)

#define LED2_ON (P4OUT |= BIT7)
#define LED2_OFF (P4OUT &= ~BIT7)
#define LED2_TOGGLE (P4OUT ^= BIT7)

#define CHAVE1_PRESSIONADA ((P2IN & BIT1) == 0)
#define CHAVE2_PRESSIONADA ((P1IN & BIT1) == 0)

void configLED1();
void configLED2();
void configCHAVE1_INT();
void configCHAVE2_INT();
void esperar();



int flagCHAVE1; // Armazenar os flacos de subida/descida
int flagCHAVE2; // Armazenar os flacos de subida/descida

/**
 * main.c : Programa da questão 3 da Prova B1 - SisMic (03 Sep 2021)
 * Matheus Elyasha Lopes - 160036526
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

	// Configuracao de entrada/saida de acordo com o arquivo dado em aula
	configLED1();
	configLED2();
	configCHAVE2_INT();
	configCHAVE1_INT();

	// Habilitar interrupcoes globais (GIE = 1)
	__enable_interrupt();

	flagCHAVE1 = 0;
	flagCHAVE2 = 0;

	while (1)
	{
		if (flagCHAVE1 != 0)
		{
			esperar();	  // Espero um tempo
			P2IE |= BIT1; // Habilito a interrupcao local
			flagCHAVE1 = 0;
		}
		if (flagCHAVE2 != 0)
		{
			esperar();	  // Espero um tempo
			P1IE |= BIT1; // Habilito a interrupcao local
			flagCHAVE2 = 0;
		}
		if (((P2IN & BIT1) != 0) & ((P1IN & BIT1) != 0))
		{
			// Caso as duas chaves estejam fechadas, apaga os LEDs
			LED1_OFF;
			LED2_OFF;
			flagCHAVE1 = 0;
			flagCHAVE2 = 0;
			esperar();
			P1IE |= BIT1;
			P2IE |= BIT1;
		}
	}

	return 0;
}

//LED vermelho, P1.0
void configLED1()
{
	P1SEL &= ~BIT0;
	P1DIR |= BIT0;
	LED1_OFF;
}

//LED verde, P4.7
void configLED2()
{
	P4SEL &= ~BIT7;
	P4DIR |= BIT7;
	LED2_OFF;
}

//Chave da esquerda, P2.1
void configCHAVE1_INT()
{
	P2SEL &= ~BIT1;
	P2DIR &= ~BIT1;
	P2REN |= BIT1;
	P2OUT |= BIT1;

	P2IES |= BIT1;
	P2IE |= BIT1;

	do
	{
		P2IFG = 0;
	} while (P2IFG != 0);
}

//Chave da direita, P1.1
void configCHAVE2_INT()
{
	P1SEL &= ~BIT1;
	P1DIR &= ~BIT1;
	P1REN |= BIT1;
	P1OUT |= BIT1;

	P1IES |= BIT1;
	P1IE |= BIT1;

	do
	{
		P1IFG = 0;
	} while (P1IFG != 0);
}

void esperar()
{
	volatile unsigned int x = 30000;
	while (x--);
}

#pragma vector = PORT2_VECTOR
__interrupt void INTERRUPCAO_PORTA2()
{
	switch (_even_in_range(P2IV, 0x10)) // Switch no interruption vector da porta 2
	{
	case 0:
		break;
	case 2:
		break;
	case 4:
		// Chave S1 foi pressionada
		if ((P2IES & BIT1) != 0)
		{
			// Flanco de descida
			flagCHAVE1 = 1;
			P2IES &= ~BIT1;
			P2IE &= ~BIT1;
			if ((P4OUT & BIT7) != 0)
			{
			}
			else
			{
			    // Vamos acender o LED1 somente se o outro LED estiver desligado
				LED1_ON;
			}
		}
		else
		{
			// Flanco de subida
			flagCHAVE1 = 2;
			P2IES |= BIT1;
			P2IE &= ~BIT1;
		}
		break;
	case 6:
		break;
	case 8:
		break;
	case 10:
		break;
	case 12:
		break;
	case 14:
		break;
	case 16:
		break;
	default:
		break;
	}
}

#pragma vector = PORT1_VECTOR
__interrupt void INTERRUPCAO_PORTA1()
{
	switch (_even_in_range(P1IV, 0x10)) // Switch no Interruption Vector da porta 1
	{
	case 0:
		break;
	case 2:
		break;
	case 4:
		// Chave S2 foi pressionada
		if ((P1IES & BIT1) != 0)
		{
			// Flanco de descida
			flagCHAVE2 = 1;
			P1IES &= ~BIT1;
			P1IE &= ~BIT1;
			if ((P1OUT & BIT0) != 0)
			{
			}
			else
			{
			    // Vamos acender o LED1 somente se o outro LED estiver desligado
				LED2_ON;
			}
		}
		else
		{
			// Flanco de subida
			flagCHAVE2 = 2;
			P1IES |= BIT1;
			P1IE &= ~BIT1;
		}
		break;
	case 6:
		break;
	case 8:
		break;
	case 10:
		break;
	case 12:
		break;
	case 14:
		break;
	case 16:
		break;
	default:
		break;
	}
}
