#include <msp430.h> 


/**
 * A2_160036526.c
 */


// Assinatura das funções que são usadas no programa
void io_config(void);
void debounce(int);
int monitoraS1(void);
int monitoraS2(void);




// Matheus Elyasha Lopes - 160036526
// Exercicio muito legal ;)

int main(void)
{
    int number_of_correct_digits = 0; // Número correto de digitos da senha
    int number_of_digits = 0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    io_config(); // Chama a configuração GPIO

    while(1)
    {
        // Testar se houve uma tentativa de senha
        // Monitora S1 para saber se está inserindo a  senha
        if(monitoraS1() == 1) {
            // Caso seja pressionada a chave S1, vamos ler S2
            switch(number_of_digits) {
                case 0: // Não houve nenhum digito
                    if (monitoraS2() == 1) {
                        // Digito errado
                        number_of_digits++;

                    } else {
                        number_of_correct_digits++; // O digito foi correto
                        number_of_digits++; // Aumenta 1 no digits
                    }
                    // Apagar o led L1 (vermelho) pois é uma nova tentativa
                    P1OUT &= ~BIT0;
                    break;

                case 1: // Houve somente 1 digito
                    if (monitoraS2() == 1) {
                        number_of_digits++;
                        number_of_correct_digits++; // O digito foi correto
                    } else {
                        number_of_digits++;
                    }
                    break;

                case 2: // Houve 2 digitos
                    if (monitoraS2() == 1) {
                        number_of_digits++;
                        number_of_correct_digits++; // O digito foi correto
                    } else {
                        number_of_digits++;
                    }
                    break;

                case 3: // Houve 3 digitos - vou testar a senha
                    if (monitoraS2() == 1) {
                        number_of_digits++;

                        // Senha incorreta
                        // Não fazer nada

                    } else {
                        number_of_digits++;
                        number_of_correct_digits++; // O digito foi correto

                        // Senha correta (testar senha)
                        // Trocar o estado do led L2 (verde) pois vamos abrir o cofre caso esteja fechado e vamos fechar o cofre caso esteja aberto.
                        if (number_of_correct_digits == 3) {
                           P4OUT ^= BIT7; // Inverter
                        }
                    }
                    // Zero os contadores para reiniciar o processo
                    number_of_digits = 0;
                    number_of_correct_digits = 0;
                    // Ligar o led L1 (vermelho) pois houve tentativa de senha
                    P1OUT |= BIT0;
                    break;

                default:
                    number_of_digits = 0;
                    number_of_correct_digits = 0;
                    break;
            }
        }

    }



    return 0;
}


void debounce(int valor)
{
    volatile int x;
    for(x=0; x<valor; x++);
}


void io_config(void)
{
    // BIT0 = 00000001
    // BIT1 = 00000010
    // BIT2 = 00000100
    // BIT3 = 00001000
    // ...
    // BIT7 = 10000000

     // ~BIT0 = 11111110

     //Led 1 em P1.0
    //P1DIR = P1DIR | BIT0; // Defino P1.0 como saída
    P1DIR |= BIT0;
    // P1OUT |= BIT0; // Acender o led L1
    P1OUT &= ~BIT0; // Apaga o led L1 - Inicia apagado pois não houve tentativa de senha no inicio do programa

     //Led 2 em P4.7
    P4DIR |= BIT7;
    // P4OUT |= BIT7; // Acender o led L2
    P4OUT &= ~BIT7; // Apaga o led L2 - Inicia apagado pois o cofre inicia trancado

     //Chave S1 em P2.1
    P2DIR &= ~BIT1; // Defino o P2.1 como entrada
    P2REN |= BIT1; // Habilito o resistor de pull-(up ou down)
    P2OUT |= BIT1; // Configuro com pull-up

     //Chave S2 em P1.1
    P1DIR &= ~BIT1; // Defino o P1.1 como entrada
    P1REN |= BIT1; // Habilito o resistor de pull-(up ou down)
    P1OUT |= BIT1; // Configuro com pull-up
}

int monitoraS1(void)
{
    static int passadoS1 = 1;

     if ((P2IN&BIT1) == 0) // pressionei o botão S1
    {
    if (passadoS1 ==1)
    {
    debounce(100);
    passadoS1 = 0;
    return 1;
    }
    }
    else
    {
    debounce(100);
    passadoS1 = 1;
    return 0;
    }

     return 0;
}

int monitoraS2(void)
{
    static int passadoS2 = 1;

    if ((P1IN&BIT1) == 0) // pressionei o botão S2
    {
    if (passadoS2 ==1)
    {
    debounce(100);
    passadoS2 = 0;
    return 1;
    }
    }
    else
    {
    debounce(100);
    passadoS2 = 1;
    return 0;
    }

     return 0;
}
