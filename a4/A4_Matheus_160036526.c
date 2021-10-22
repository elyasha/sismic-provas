/*
 * Matheus Elyasha Lopes - 160036526
 * Data de criacao: 14 Outubro 2021
 * Link do repositorio para acesso posterior
 * https://github.com/elyasha/sismic-provas
 * O codigo LIGA os resistores de pull-up internos. Apenas ligar o SDA e SCL (jumpers no msp direto)
 *
 */

#include <msp430.h>
#define LED1_ON (P1OUT |= BIT0)
#define LED1_OFF (P1OUT &= ~BIT0)
#define LED1_TOGGLE (P1OUT ^= BIT0)

#define LED2_ON (P4OUT |= BIT7)
#define LED2_OFF (P4OUT &= ~BIT7)
#define LED2_TOGGLE    (P4OUT ^= BIT7

#define SMCLK 1048576L //Freq do SMCLK
#define ACLK 32768     //Freq do ACLK

//O objetivo e encontrar essas duas variaveis: o endereco do SLAVE e a SENHA.
#define ADDRESS 0x05
#define DATA 0x0F

#define TRUE 1
#define FALSE 0

#define ESCR_WR 0 //Enderecar Escravo para escrever
#define BR10K 105 //com SMCLK

// - UCB1 e configurado como SLAVE RECEIVER
//Hardware setup:
// - P3.0 - P4.1 - SDA
// - P3.1 - P4.2 - SCL
// - O codigo LIGA os resistores de pull-up internos do RECEIVER (voce pode ligar os resistores do master)
// O receiver usa o LED VERDE quando ele recebe o byte correto.
void initialize_I2C_UCB1_Slave();

void usci_b0_config(void);
void gpio_config(void);

unsigned char byteReceived;

char i2c_test(char adr, char modo);
void teste_nack(void);

void delay_us(unsigned int time_us);

/**
 * main.c: Solucao do A4 (testByte é a senha e slavePosition é o endereço)
 */
int main(void)
{
    unsigned char adr; //Endereco do escravo
    unsigned char testByte;
    unsigned char slavePosition;
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    gpio_config();
    initialize_I2C_UCB1_Slave();
    usci_b0_config();
    __enable_interrupt(); //Habilitacao geral (GIE = 1)

    // Procurar o escravo receptor em todos os enderecos de 7 bits (0 - 127)
    for (adr = 0; adr < 127; adr++)
    {
        if (i2c_test(adr, ESCR_WR) == TRUE)
        {
            slavePosition = adr; // Encontrei o escravo e salvo o endereco
            break;
        }
    }

    // Vou fazer um brute force para encontrar a senha (1 byte = 0 até 255)
    for (testByte = 0; testByte < 256; testByte++)
    {
        // Mandar byte para o escravo
        UCB0I2CSA = slavePosition;  //Endereco do escravo
        UCB0CTL1 |= UCTR | UCTXSTT; //Gerar START
        while ((UCB0IFG & UCTXIFG) == 0); //Esperar TXIFG=1 (START)
        UCB0TXBUF = testByte; // Payload no TXBUF
        while ((UCB0CTL1 & UCTXSTT) == UCTXSTT); //Esperar TXSTT=0 (confirmacao)
        teste_nack();
        UCB0CTL1 |= UCTXSTP; //Gerar STOP
        while ((UCB0CTL1 & UCTXSTP) == UCTXSTP); //Esperar STOP


        // Verificar se o led verde está aceso
        if ((P4OUT & BIT7) == 0x0080)
        {
            break;
        }
    }

    while (TRUE); // Prender execucao

    return 0;
}

//Verificar se aconteceu NACK
void teste_nack(void)
{
    if ((UCB0IFG & UCNACKIFG) == UCNACKIFG)
    {
        P1OUT |= BIT0;       //Vermelho=falha
        UCB0CTL1 |= UCTXSTP; //Gerar STOP
        while ((UCB0CTL1 & UCTXSTP) == UCTXSTP); //Esperar STOP
        while (1); //Travar
    }
}

// Testar o endereco adr para escrita ou leitura
// modo = ESCR_RD --> Enderecar escravo para leitura
// modo = ESCR_WR --> Enderecar escravo para escrita
char i2c_test(char adr, char modo)
{
    UCB0I2CSA = adr; //Endereco a ser testado
    if (modo == ESCR_WR)
    {                        //WR = Escravo receptor
        UCB0CTL1 |= UCTR;    //Mestre TX --> escravo RX
        UCB0CTL1 |= UCTXSTT; //Gerar STASRT
        while ((UCB0IFG & UCTXIFG) == 0); //TXIFG=1, START iniciado
    }
    else
    {                         //RD = Escravo transmissor
        UCB0CTL1 &= ~UCTR;    //Mestre RX <-- escravo TX
        UCB0CTL1 |= UCTXNACK; //NACK ao receber um dado
        UCB0CTL1 |= UCTXSTT;  //Gerar START
        while ((UCB0CTL1 & UCTXSTT) == UCTXSTT); //Esperar START
    }
    UCB0CTL1 |= UCTXSTP; //Gerar STOP
    while ((UCB0CTL1 & UCTXSTP) == UCTXSTP); //Esperar STOP
    //Teste do ACK
    if ((UCB0IFG & UCNACKIFG) == 0)
        return TRUE; //Chegou ACK
    else
        return FALSE; //Chegou NACK
}

void gpio_config(void)
{
    // GPIO Config
    P4DIR |= BIT7;  //(Verde) P4.7 = saida
    P4OUT &= ~BIT7; //Verde apagado
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
}

void usci_b0_config(void)
{
    // Resetar USCI_B0
    UCB0CTL1 = UCSWRST;

    // Configura os pinos e o USCI_B1
    UCB0CTL0 = UCMST |    // Modo Mestre
               UCMODE_3 | // I2C
               UCSYNC;    // Sincrono

    UCB0BRW = BR10K; //10 kbps

    UCB0CTL1 = UCSSEL_3; //SMCLK e UCSWRST=0

    P3SEL |= BIT1 | BIT0; // Funces alternativas
    P3REN |= BIT1 | BIT0; // Habilitar resistor
    P3OUT |= BIT1 | BIT0; // Pullup

    // Habilitar interrupcoes
}

/*
 * Delay microsseconds.
 */
void delay_us(unsigned int time_us)
{
    //Configure timer A0 and starts it.
    TA0CCR0 = time_us;
    TA0CTL = TASSEL__SMCLK | ID__1 | MC_1 | TACLR;

    //Locks, waiting for the timer.
    while ((TA0CTL & TAIFG) == 0);

    //Stops the timer
    TA0CTL = MC_0 | TACLR;
}

/*
 *  P4.1 - SDA
 *  P4.2 - SCL
 */
void initialize_I2C_UCB1_Slave()
{
    //Configura o LED2.
    P4SEL &= ~BIT7;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    //Desliga o modulo
    UCB1CTL1 |= UCSWRST;

    //Configura os pinos
    P4SEL |= BIT1; //Configuro os pinos para "from module"
    P4SEL |= BIT2;
    P4REN |= BIT1;
    P4REN |= BIT2;
    P4OUT |= BIT1;
    P4OUT |= BIT2;

    UCB1CTL0 = UCMODE_3 | //I2C Mode
               UCSYNC;    //Synchronous Mode

    UCB1CTL1 = UCSSEL__ACLK | //Clock Source: ACLK
               UCSWRST;       //Mantem o modulo desligado

    //Escrevo meu endereco
    UCB1I2COA = ADDRESS;

    //Prepara minhas variaveis.
    byteReceived = 0;

    //Liga o modulo.
    UCB1CTL1 &= ~UCSWRST;

    //Liga a interrupcao de RX.
    UCB1IE = UCRXIE;

    _enable_interrupt();
}

#pragma vector = USCI_B1_VECTOR;
__interrupt void i2c_isr()
{
    switch (__even_in_range(UCB1IV, 12))
    {
    case USCI_NONE:
        break;
    case USCI_I2C_UCALIFG:
        break;
    case USCI_I2C_UCNACKIFG:
        break;
    case USCI_I2C_UCSTTIFG:
        break;
    case USCI_I2C_UCSTPIFG:
        break;
    case USCI_I2C_UCRXIFG:
        byteReceived = (unsigned char)UCB1RXBUF;
        volatile char d = DATA;
        if (byteReceived == d)
        {
            P4OUT |= BIT7;
        }
        else
        {
            P4OUT &= ~BIT7;
        }
        break;
    case USCI_I2C_UCTXIFG:
        break;

    default:
        break;
    }
}
