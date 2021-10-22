#include <msp430.h>

// Variaveis globais
unsigned int sequence[10];
unsigned int mynumber = 0;

// Assinaturas das funcoes
void tempo();
unsigned int lfsr();
void start();
void createSequence(int n);
void showSequence(int n);
int game(int n);
void youwon();
void youlose();
void io_config(void);
int monitoraS1(void);
int monitoraS2(void);
void debounce(int valor);


/**
 * main.c : Programa da questão 2 da Prova B1 - SisMic (03 Sep 2021)
 * Matheus Elyasha Lopes - 160036526
 */
int main(void)
{
     WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    io_config();

    start();

    // createSequence(2);

    // showSequence(2);

    // Pensei numa forma de refatorar esses ifs aninhados, bastaria criar uma variavel level e ir fazer um loop for

    if (game(2) == 0)
    {
        youlose();
        main();
    }
    else
    {
        youwon();
        start();
        if (game(3) == 0)
        {
            youlose();
            main();
        }
        else
        {
            youwon();
            start();
            if (game(4) == 0)
            {
                youlose();
                main();
            }
            else
            {
                youwon();
                start();
                if (game(5) == 0)
                {
                    youlose();
                    main();
                }
                else
                {
                    youwon();
                    start();
                    if (game(6) == 0)
                    {
                        youlose();
                        main();
                    }
                    else
                    {
                        youwon();
                        start();
                        if (game(7) == 0)
                        {
                            youlose();
                            main();
                        }
                        else
                        {
                            youwon();
                            start();
                            if (game(8) == 0)
                            {
                                youlose();
                                main();
                            }
                            else
                            {
                                youwon();
                                start();
                                if (game(9) == 0)
                                {
                                    youlose();
                                    main();
                                }
                                else
                                {
                                    youwon();
                                    start();
                                    if (game(10) == 0)
                                    {
                                        youlose();
                                        main();
                                    }
                                    else
                                    {
                                        while (1)
                                        {
                                            // Parabéns, você zerou o jogo e o led verde ficará piscando para sempre
                                            youwon();
                                        };
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };

    return 0;
}

void start()
{
    // A rotina deve acender os dois LEDs por um certo tempo,
    // Ligar os leds
    P1OUT |= BIT0;
    P4OUT |= BIT7;
    tempo();
    tempo();
    tempo();

    // manter o LED verde aceso por um certo tempo
    P1OUT &= ~BIT0;
    tempo();
    tempo();
    tempo();

    // manter os dois LEDS apagados por um certo tempo
    P4OUT &= ~BIT7;
    tempo();
    tempo();
    tempo();
};

void createSequence(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sequence[i] = lfsr();
    }
};

void showSequence(int n)
{
    createSequence(n);
    int i;

    for (i = 0; i < n; i++)
    {
        if (sequence[i] == 0)
        {
            // Acender o led vermelho por um tempo
            P1OUT |= BIT0;
            tempo();
            P1OUT &= ~BIT0;
        }
        else
        {
            // Acender o led verde por um tempo
            P4OUT |= BIT7;
            tempo();
            P4OUT &= ~BIT7;
        }

        // Apagar os dois leds por um tempo
        P1OUT &= ~BIT0;
        P4OUT &= ~BIT7;
        tempo();
    }
};

int game(int n)
{
    showSequence(n);
    unsigned int number_of_correct_numbers = 0; // Número de vezes que ele acertou a sequência até o n-ésimo led
    // Vamos pegar o input do usuário, caso ele tecle algo errado, retornamos zero, caso contrário, retornamos 1

    // O jogo funciona, porém houve algumas coisas com o tempo do debounce dos botões, seria melhor testar para encontrar
    // um melhor casamento de tempo para ler os leds ou mesmo usar interrupções que deixei para a questão 3

    while (number_of_correct_numbers < n)
       {
           if (monitoraS1() == 1) {
               // Caso o led correto seja vermelho
               if (sequence[n] == 0) {
                   number_of_correct_numbers++;
                   continue;
               } else {
                   break;
               };

           }

           if (monitoraS2() == 1) {
               // Caso o led correto seja verde
               if (sequence[n] == 1) {
                   number_of_correct_numbers++;
                   continue;
               } else {
                   break;
               };
           };

       };
       if (number_of_correct_numbers == n) {
           number_of_correct_numbers = 0;
           return 1;
       }

       return 0;

};

void youwon() //Sequencia correta
{
    tempo();
    int i;
    // Piscar o led 5 vezes
    for (i = 0; i < 5; i++)
    {
        // Ligar o led verde
        P4OUT |= BIT7;
        tempo();

        // Desligar o led verde
        P4OUT &= ~BIT7;
        tempo();
    }
};

void youlose() //Sequencia Incorreta
{
    tempo();
    int i;
    // Piscar o led 5 vezes
    for (i = 0; i < 5; i++)
    {
        // Ligar o led verde
        P1OUT |= BIT0;
        tempo();

        // Desligar o led verde
        P1OUT &= ~BIT0;
        tempo();
    }
};

void io_config(void)
{

    //Led 1 em P1.0
    //P1DIR = P1DIR | BIT0; // Defino P1.0 como saida
    P1DIR |= BIT0;

    //Led 2 em P4.7
    P4DIR |= BIT7;

    //Chave S1 em P2.1
    P2DIR &= ~BIT1; // Defino o P2.1 como entrada
    P2REN |= BIT1;  // Habilito o resistor de pull-(up ou down)
    P2OUT |= BIT1;  // Configuro com pull-up

    //Chave S2 em P1.1
    P1DIR &= ~BIT1; // Defino o P1.1 como entrada
    P1REN |= BIT1;  // Habilito o resistor de pull-(up ou down)
    P1OUT |= BIT1;  // Configuro com pull-up
}

void debounce(int valor)
{
    volatile int x;
    for(x=0; x<valor; x++);
}

void tempo()
{
    volatile unsigned int x = 50000;
    while (x--)        ;
}

unsigned int lfsr()
{
    mynumber++;
    unsigned int x = (mynumber % 3);
    return (x % 2);
}

int monitoraS1(void)
{
    static int passadoS1 = 1;

    if ((P2IN & BIT1) == 0) // pressionei o botão S1
    {
        if (passadoS1 == 1)
        {
            debounce(1000);
            passadoS1 = 0;
            return 1;
        }
    }
    else
    {
        debounce(1000);
        passadoS1 = 1;
        return 0;
    }

    return 0;
}

int monitoraS2(void)
{
    static int passadoS2 = 1;

    if ((P1IN & BIT1) == 0) // pressionei o botão S2
    {
        if (passadoS2 == 1)
        {
            debounce(1000);
            passadoS2 = 0;
            return 1;
        }
    }
    else
    {
        debounce(1000);
        passadoS2 = 1;
        return 0;
    }

    return 0;
}
