// Funções relacionadas aos componentes de entrada, ligados ao GPIO da DE1-SoC

#include "GPIO.h"

// Declaração de Variáveis Globais:
void* GPIO_VIRTUAL;
volatile unsigned int* KEYS_BASE;
volatile unsigned int* HEX5_BASE;
volatile unsigned int* HEX4_BASE;
volatile unsigned int* HEX3_BASE;
volatile unsigned int* HEX2_BASE;
volatile unsigned int* HEX1_BASE;
volatile unsigned int* HEX0_BASE;

// Virtualizando o Endereço Físico do GPIO:
void GPIO_mapping(void){
    size_t GPIO0_SPAN = 4096;
    off_t GPIO0_OFST = 0xff200000;
    GPIO_VIRTUAL = mmap(NULL, GPIO0_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_OFST);
    KEYS_BASE = GPIO_VIRTUAL + 0x0;  // Botões
    HEX5_BASE = GPIO_VIRTUAL + 0x10; // HEX5: Digito apagado
    HEX4_BASE = GPIO_VIRTUAL + 0x20; // HEX4: Digito da dezena
    HEX3_BASE = GPIO_VIRTUAL + 0x30; // HEX3: Digito da unidade
    HEX2_BASE = GPIO_VIRTUAL + 0x40; // HEX2: Digito apagado
    HEX1_BASE = GPIO_VIRTUAL + 0x50; // HEX1: Digito apagado
    HEX0_BASE = GPIO_VIRTUAL + 0x60; // HEX0: Digito apagado
}

int exibir_pontos(int pontos)
{
    // Digitos que não seram utilizados são apagados
    *HEX5_BASE = 0xff;
    *HEX2_BASE = 0xff;
    *HEX1_BASE = 0xff;
    *HEX0_BASE = 0xff;

    int* decomposto = return_decomposto(pontos);

    // Definindo as unidades:
    if (decomposto[1] == 0)
        *HEX3_BASE = 0xc0; // Definindo unidade zerada
    else if (decomposto[1] == 1)
        *HEX3_BASE = 0xf9; // Definindo unidade 1
    else if (decomposto[1] == 2)
        *HEX3_BASE = 0x1a4; // Definindo unidade 2
    else if (decomposto[1] == 3)
        *HEX3_BASE = 0xb0; // Definindo unidade 3
    else if (decomposto[1] == 4)
        *HEX3_BASE = 0x99; // Definindo unidade 4
    else if (decomposto[1] == 5)
        *HEX3_BASE = 0x92; // Definindo unidade 5
    else if (decomposto[1] == 6)
        *HEX3_BASE = 0x82; // Definindo unidade 6
    else if (decomposto[1] == 7)
        *HEX3_BASE = 0xf8; // Definindo unidade 7
    else if (decomposto[1] == 8)
        *HEX3_BASE = 0x80; // Definindo unidade 8
    else if (decomposto[1] == 9)
        *HEX3_BASE = 0x90; // Definindo unidade 9
    else{
        *HEX3_BASE = 0xc0;
    }

    // Definindo as dezenas:
    if (decomposto[0] == 0)
        *HEX4_BASE = 0xc0; // Definindo dezena zerada
    else if (decomposto[0] == 1)
        *HEX4_BASE = 0xf9; // Definindo dezena 10
    else if (decomposto[0] == 2)
        *HEX4_BASE = 0x1a4; // Definindo dezena 20
    else if (decomposto[0] == 3)
        *HEX4_BASE = 0xb0; // Definindo dezena 30
    else if (decomposto[0] == 4)
        *HEX4_BASE = 0x99; // Definindo dezena 40
    else if (decomposto[0] == 5)
        *HEX4_BASE = 0x92; // Definindo dezena 50
    else if (decomposto[0] == 6)
        *HEX4_BASE = 0x82; // Definindo dezena 60
    else if (decomposto[0] == 7)
        *HEX4_BASE = 0xf8; // Definindo dezena 70
    else if (decomposto[0] == 8)
        *HEX4_BASE = 0x80; // Definindo dezena 80
    else if (decomposto[0] == 9)
        *HEX4_BASE = 0x90; // Definindo dezena 90
    else{
        *HEX4_BASE = 0xc0;
    }
    return 0;
}

/**
* Lê e retorna os valores lógicos dos botões:
* 15 (0xF) - Botões Desligados - Return 0
* 14 (0xE) - KEY0 - Botão 0 Pressionado - Return 1
* 13 (0xD) - KEY1 - Botão 1 Pressionado - Return 2
* 11 (0xB) - KEY2 - Botão 2 Pressionado - Return 3
* 07 (0x7) - KEY3 - Botão 3 Pressionado - Return 4
*/
int button()
{
    int botao;
    if (*KEYS_BASE == 0xF){
        botao = 0;
    }
    else if (*KEYS_BASE == 0xE){
        botao = 1;
    }
    else if (*KEYS_BASE == 0xD){
        botao = 2;
    }
    else if (*KEYS_BASE == 0xB){
        botao = 3;
    }
    else if (*KEYS_BASE == 0x7){
        botao = 4;
    }
    else{
        botao = 0; // Se mais de um botão estiver pressionado.
    }
    return botao;
}

int* return_decomposto(int pontos) //Função de retorno da pontuação decomposta. A pontuação máxima foi definida como 99;
{
    static int tamanho = 2;
    int* dVetor = (int*)malloc(tamanho * sizeof(int)); // Define o vetor que irá guardar os valores decompostos dos pontos
    if(pontos>=0 && pontos < 10){
        dVetor[0] = 0; 
        dVetor[1] = pontos; // Definindo a unidade;
    }
    else if(pontos >= 10 && pontos <= 99){
        //Preenchendo o vetor "decomposto" com dezena e unidade;
        dVetor[1] = pontos % 10; // Definindo a unidade;   
        dVetor[0] = pontos/10; // Definindo a dezena;
    }
    else{
        dVetor[0] = 0;
        dVetor[1] = 0; 
    }
    return dVetor;
}
