// Funções de Jogabilidade do Tetris

#ifndef JOGABILIDADE_H
#define JOGABILIDADE_H

// Chamadas das Bibliotecas:
#include <stdio.h>    // Entrada e Saída, como "printf"
#include <stdlib.h>   // Funções "rand" e "srand"
#include <fcntl.h>    // Para Manipulação de Arquivos
#include <unistd.h>   // Para Fechar Arquivos
#include <time.h>     // Função "sleep"
#include <stdint.h>   // Para Inteiros de 8 e 16 bits
#include "sys/mman.h" // Para Mapear Endereço Físico em Endereço Virtual
#include "vga.h"      // Instruções Assembly do VGA
#include "ADXL345.h"  // Funções do Acelerômetro
#include "GPIO.h"     // Funções relacionadas aos componentes de entrada, ligados ao GPIO da DE1-SoC

// Definindo Parâmetros do Ambiente de Jogo:
#define LINHA 20
#define COLUNA 10

// Variáveis que serão usadas em outras bibliotecas:
extern int pontuacao;
extern int indice_peca;
extern int coluna;
extern int linha;

// Cabeçalho das Funções:
void draw_start_screen();                         // Desenha a tela inicial do jogo
int desenharBloco(int coluna, int linha, int color);
int apagar_especifico(int linha, int coluna);
int apagar();
void desenharCampo();
void fixarPeca();                                 // Deixar a peça no tabuleiro
void checarLinhas();
int descida();                                    //Controlar a queda
void mover(int lado);                             //Função que move a peça -1 esquerda,+1 direita
int colisao(int x, int y, int indice_peca_atual); //Verifica se a posição (i, j) da peça está ocupada, x - coluna, y - linha

#endif // JOGABILIDADE_H