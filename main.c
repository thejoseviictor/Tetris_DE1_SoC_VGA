#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h> // Entrada e Saída, como "printf"
#include <stdlib.h>
#include "fcntl.h" // Para Manipulação de Arquivos
#include "unistd.h" // Para Fechar Arquivos
#include "time.h" // Função "sleep"
#include "sys/mman.h"// Para Mapear Endereço Físico em Endereço Virtual
#include <time.h>
#include "ADXL345.h" // Funções do Acelerômetro

// Funções para Virtualização de Memória:
//extern void * physical_to_virtual();
extern int unmap_memory(void*, size_t);

// Definindo as Cores:
#define COLOR_RED 0x07
#define COLOR_GREEN 0x38
#define COLOR_BLUE 0x1C0
#define COLOR_YELLOW 0x3F
#define COLOR_CYAN 0x1F8
#define COLOR_MAGENTA 0x1C7
#define COLOR_WHITE 0x1FF
#define COLOR_BLACK 0x00
#define COLOR_SILVER 0x16D
#define COLOR_GRAY 0xDB

#define LINHA 20
#define COLUNA 10

void* virtual;

#include "return_decomposto.h"
#include "exibir_pontos.h"


// Iniciando o Acelerômetro:



int borda[LINHA][COLUNA]={0};
int pontuacao =0;
int pecas[6][4][4]={
    {{0,0,0,0},{1,1,1,1}},
    {{0,1,1,0},{0,1,1,0}},
    {{0,1,0,0},{1,1,1,0}},
    {{0,0,0,1},{0,1,1,1}},
    {{1,1,0},{0,1,1}},
    {{0,1},{1,1},{1,0}}
};

int indice_peca= 0;
int coluna=3; //Coluna no tabuleiro, posição horizontal
int linha=0; // linha no tabuleiro, posição vertical

//Instruções do VGA:
extern void* background_color(void*, int);  //Instrução WBR;
extern void* background_drawing(void*, int, int, int); //Instrução WBM;
extern void buffer_overflow(void*); //Instrução WBM;
extern void draw_polygon(void*, int x, int y, int size, int color, int type);  //Instrução DP;
extern void* set_sprite(void*, int, int x, int y, int sp);  //Instrução WBR;

int desenharBloco(void *virtual,int coluna, int linha, int color){
    buffer_overflow(virtual);
    background_drawing(virtual, linha, coluna, color); 
}


int apagar_especifico(void *virtual, int linha, int coluna){
    background_drawing(virtual, linha, coluna, COLOR_BLACK); 
    return 0;
}


int apagar(void *virtual){
      for (int coluna = 0; coluna <80; coluna++){
        for (int linha = 0; linha < 60; linha ++){
            buffer_overflow(virtual);
            background_drawing(virtual, linha, coluna, COLOR_BLACK); 
        }
    }

}


void desenharCampo(){
    //Limpa a tela primeiro
    apagar(virtual);
    
    //loop para  percorrer todas as linhas para desenhar as bordas laterais
    for(int y=0; y <= LINHA; y++){
        desenharBloco(virtual,0,y,COLOR_BLUE); //a borda esquerda
        desenharBloco(virtual,COLUNA+1,y,COLOR_BLUE); // a borda direita 
    }   

    //percorre todas as colunas para desenhar a borda base
    for(int x=0;x<=COLUNA+1; x++){
        desenharBloco(virtual,x,LINHA,COLOR_BLUE); //Lembrando que LINHA é a ultima linha 
    }

    //laço para percorrer toda a matriz para fazer os blocos fixo nela
    for (int y=0; y< LINHA; y++){
        for (int x=0; x<COLUNA; x++){
            if(borda[y][x]){  // No caso, se tiver um valor positivo, ou seja, se tiver um bloco 
                //Verificando se no ponto atual na matriz o valor é 1, ou seja, tem peça
                desenharBloco(virtual,x+1,y,COLOR_GRAY);
            }
        }
    }\

    //Desenha o bloco da peça na posição correta do tabuleiro, passando por toda a matriz do bloco
    for(int i=0; i<4;i++){
        for(int j=0; j<4;j++){
            if(pecas[indice_peca][i][j]){ //Se tiver um valor positivo, eu desenho
                //verificar que se na posição atual tem algum bloco, se sim ele precisa ser desenhado 
                desenharBloco(virtual,coluna+ j, linha + i, COLOR_GREEN);
            }
        }
    
    
    }}



// Deixar a peça no tabuleiro
void fixarPeca(){
    //Um laço que percorre as linhas da peça atual
    //Verifica se a posição atual da peça contém um bloco
    for(int i=0; i<4; i++){
        for(int j =0; j<4; j++){
            if(pecas[indice_peca][i][j]){
                // Aqui eu só coloco na matriz borda os valores positivos onde a peça estava, ou seja, valores fixos
                borda[linha + i][coluna+ j]=1; //atualizaçao da matriz borda, marcando a posição da peça no tabuleiro
            }
        }
    }
}



void checarLinhas(){
    //Percorre todas as linhas do tabuleiro, ver tá completa
    for(int y = 0; y < LINHA; y++){
        //verificando linhas
        int linha_completa = 1;
        for(int x =0; x< COLUNA; x++){
            // se eu  achar na coluna um só zero eu não preciso verificar mais as outras colunas 
            if(borda[y][x] == 0){
                linha_completa=0;
                break;
            }
        }

        //Se tiver alguma linha completa, a variavel linha_completa permanecerá com o valor 1
        if(linha_completa){
            //Se a linha está completa, remove ela e atualiza pontuação.
            pontuacao+=1;
            
        
            //Descendo as coisas que tão em cima
            for(int i = y; i > 0; i--){
                for(int j = 0; j < COLUNA; j++){
                    borda[i][j] = borda[i-1][j];     
                }
            }

            for ( int j = 0; j < COLUNA; j++){ //Removendo blocos da linha, coluna por coluna
                borda[0][j]=0;
            }
            y--; //Reajusta o valor de y(linha) para verificar novamente a linha atual, já que ela foi substituída.

        }
    }
}

//Controlar a queda
int descida() {
    if(!colisao(coluna,linha+1, indice_peca)){
        //Verifica se a peça pode descer uma linha sem colidir com outra peça ou a base
        linha++; // Move o bloco para baixo
        return 0; //peça não precisa ser fixada ainda
    }
    else{ //Se não eu fixo a peça lá mesmo e logo após eu verifico se completou a linha
        fixarPeca();
        checarLinhas();
        return 1;
    }
}

//Função que move a peça -1 esquerda,+1 direita
void mover(int lado){
    int x= coluna+ lado;

    //verificar colisão com a borda
    if( !colisao(x,linha,indice_peca)){
        coluna= x; //Se não tiver colisão com a borda lateral a peça move
    }

    //Se  tiver colisão com a borda lateral a peça fica no mesmo lugar
}

//Verifica se a posição (i, j) da peça está ocupada, x - coluna, y - linha
int colisao(int x, int y, int indice_peca_atual) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {  // Percorre todos os blocos da peça
            if (pecas[indice_peca_atual][i][j]) {
                // Verifica colisão com as bordas laterais e inferiores
                if (y + i >= LINHA || x + j >= COLUNA || x + j < 0 || borda[y + i][x + j]) {
                    return 1;  // Retorna 1 se houver colisão
                }
            }
        }
    }
    return 0;  // Sem colisão
}





int main(){
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    enable_I2C0_HPS(fd);
    size_t virtual_address_size = 4096; // Tamanho dos Endereços Virtuais

    // Endereços dos Registradores do Módulo "I2C0":
    off_t I2C0_BASE = 0xFFC04000; // Endereço Físico Base do I2C0
    void * I2C0_BASE_VIRTUAL = mmap (NULL, virtual_address_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, I2C0_BASE); // Endereço Virtual Base do I2C0
    volatile unsigned int * I2C0_DATA = I2C0_BASE_VIRTUAL + 0x10; // Para Escrever ou Ler em um "Slave"
    volatile unsigned int * I2C0_RXFLR = I2C0_BASE_VIRTUAL + 0x78; // Para Verificar se Existem Dados a Receber

    setting_I2C0_ADXL345(I2C0_BASE_VIRTUAL); // Configurando o Barramento I2C0
    setting_ADXL345(I2C0_DATA); // Configurando os Sensores do Acelerômetro

    off_t ALT_LWFPGASLVS_OFST = 0xFF200000;
    size_t ALT_LWFPGASLVS_SPAN = 4096;
    virtual = mmap(NULL, ALT_LWFPGASLVS_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, ALT_LWFPGASLVS_OFST);
    apagar(virtual);
    desenharCampo();
    int16_t eixo_x; 
    int loop = 1;
    while(loop){
        
        eixo_x = lerEixoX(I2C0_DATA, I2C0_RXFLR);

        // Mover Peças para Esquerda ou Direita:
        if (eixo_x > 100)
        {
            mover(1);
        }
        else if (eixo_x < -100)
        {
            mover(-1);
        }

        if(descida()){
            indice_peca = rand() % 6; //Pegando o indice da peça
            coluna= 4; 
            linha = 0;

            //Verifica se ouve colisão 
            if(colisao(coluna,linha,indice_peca)){

                char texto_do_gameover[20];
                printf(texto_do_gameover,"GAME-OVER");

                break;
            }
        }
        desenharCampo();
        //Talvez usleep não pegue
        usleep(300 * 1000); // Controla a velocidade da queda
        exibir_pontos(pontuacao);

    }
    return 0;
}

