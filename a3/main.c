
/*
 * Matheus Elyasha Lopes - 160036526
 * Data de criacao: 21 Setembro 2021
 * Link do repositorio para acesso posterior
 * https://github.com/elyasha/sismic-provas
 */

#include <msp430.h>
#define LED1_ON (P1OUT |= BIT0)
#define LED1_OFF (P1OUT &= ~BIT0)
#define LED1_TOGGLE (P1OUT ^= BIT0)

#define LED2_ON (P4OUT |= BIT7)
#define LED2_OFF (P4OUT &= ~BIT7)
#define LED2_TOGGLE    (P4OUT ^= BIT7

#define SMCLK 1048576L //Freq do SMCLK
#define ACLK 32768	   //Freq do ACLK - Vai ser usado

#define T1ms 500		   //periodo de 1 ms
#define PWM10 (0.1 * T1ms) //10% de ciclo de carga
#define PWM30 (0.3 * T1ms) //30% de ciclo de carga
#define PWM50 (0.5 * T1ms) //50% de ciclo de carga
#define PWM70 (0.7 * T1ms) //70% de ciclo de carga
#define PWM90 (0.9 * T1ms) //90% de ciclo de carga

void gpio_config(void);
void timer_config(void);
void isr_taifg(void);
void isr_taifg1(void);

int pwmmode = 0;
int i = 0;

/**
 * main.c: Solucao do A3
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

	gpio_config();

	timer_config();
	__enable_interrupt(); //Habilitacao geral (GIE = 1)

	while (1)
		;

	return 0;
}

void timer_config(void)
{

	// Configuraao de TA1.0
	TA1CTL = TASSEL_1 | MC_1 | TACLR; //ACLK e Modo 1
	// Em 250ms temos 8192 contagens, logo
	TA1CCR0 = 8192;	 //Limite de contagem (250 ms -> 4 Hz)
	TA1CCTL0 = CCIE; //CCIFG interrompe

	//Configurao de TA0.1 e P1.2
	TA0CTL = TASSEL_1 | MC_1 | TACLR; //ACLK e Modo 1
	TA0CCR0 = T1ms;					  
	TA0CCR1 = PWM10;
	TA0CCTL1 = CCIE;	 //CCIFG interrompe
	TA0CCTL1 = OUTMOD_6; //Modo da saida
	// Config p12
	P1DIR |= BIT2; //P1.2 como saida
	P1SEL |= BIT2; //Dedicado para TA0.1
}

void gpio_config(void)
{
	// GPIO Config
	P4DIR |= BIT7;	//(Verde) P1.7 = saida
	P4OUT &= ~BIT7; //Verde apagado
}

// Interrupcao Timer
// Interrupcao TA
// #pragma vector = TIMER0_A1_VECTOR
#pragma vector = 52
__interrupt void isr_ta0(void)
{
	int n;
	n = __even_in_range(TA0IV, 0xE);
	switch (n)
	{
	case 0x0:
		break; //Nenhuma interrpcao pendente
	case 0x2:
		isr_taifg();
		break; //TA0CCTL1.CCIFG
	case 0x4:
		break; //TA0CCTL2.CCIFG
	case 0x6:
		break; //TA0CCTL3.CCIFG
	case 0x8:
		break; //TA0CCTL4.CCIFG
	case 0xA:
		break; //TA0CCTL5.CCIFG (Nao existe no TA0)
	case 0xC:
		break; //TA0CCTL6.CCIFG (Nao existe no TA0)
	case 0xE:
		break; //TA0CTL.TAIV
	}
}

// Interrupcao Timer
// Interrupcao TA
// #pragma vector = TIMER1_A0_VECTOR
#pragma vector = 49
__interrupt void isr_ta1(void)
{

	isr_taifg1();
}

void isr_taifg(void)
{
	// NÃ£o faz nada
}

void isr_taifg1(void)
{
	P4OUT ^= BIT7; // A cada 250ms inverter LED

	if (i++ == 2) // A cada 500ms trocar o pwm mode
	{
		i = 0;

		switch (pwmmode)
		{
		case 0:
			TA0CCR1 = PWM10;
	        pwmmode++;
			break;
		case 1:
			TA0CCR1 = PWM30;
            pwmmode++;
			break;
		case 2:
			TA0CCR1 = PWM50;
            pwmmode++;
			break;
		case 3:
			TA0CCR1 = PWM70;
            pwmmode++;
			break;
		case 4:
			TA0CCR1 = PWM90;
			pwmmode = 0;
			break;
		}

	}
}
