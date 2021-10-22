#include <msp430.h> 
#include <stdio.h>


unsigned int lfsr();
extern unsigned int estado = 0xACE1; // 1010 1100 1110 0001

/**
 * main.c : Programa da questão 1 da Prova B1 - SisMic (03 Sep 2021)
 * Matheus Elyasha Lopes - 160036526
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Valores corretos para o exemplo do PDF e comparação do programa
    // N     HEX          BINARY                   DECIMAL
    // 0     0xACE1       1010 1100 1110 0001      44257
    // 1     0x5670       0101 0110 0111 0000      22128
    // 2     0xAB38       1010 1011 0011 1000      43832
    // 3     0x559C       0101 0101 1001 1100      21916
    // 4     0x2ACE       0010 1010 1100 1110      10958
    // 5     0x1567       0001 0101 0110 0111      5479
    // 6     0x8AB3       1000 1010 1011 0011      35507

    while(1) {
        lfsr(); // Loop infinito para testar as iterações
    }

    return 0; // Return 0, mas que acaba nunca chegando pois estamos num loop infinito
}

unsigned int lfsr()
{
    // Esse é o bit que será calculado a partir da relação dada no problema, é muito importante ser unsigned
    unsigned int result_bit;


    // Calcula o b para mudar o estado
    unsigned int bit1 = estado & BIT0;
    unsigned int bit2 = (estado & BIT2) >> 2;
    unsigned int bit3 = (estado & BIT3) >> 3;
    unsigned int bit4 = (estado & BIT5) >> 5;


    // b = x16 XOR x14 XOR x13 XOR x11
    result_bit = bit1 ^ bit2 ^ bit3 ^ bit4;

    // Mudar o estado (Shift para direita)
    estado >>= 1;

    // Colocar o bit na casa correta
    result_bit <<= 15;

    // Somar o estado com o bit para setar o bit mais significativo
    estado += result_bit;

    // Voltar o bit para 0 ou 1
    result_bit >>= 15;


    return result_bit;

};

