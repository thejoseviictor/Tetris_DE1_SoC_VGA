// Funções de Jogabilidade do Tetris

#include "jogabilidade.h"

// Declaração de Variáveis Globais:
int borda[LINHA][COLUNA] = {0};
int pontuacao = 0;
int pecas[6][4][4] = {
    {{0,0,0,0},{1,1,1,1}},
    {{0,1,1,0},{0,1,1,0}},
    {{0,1,0,0},{1,1,1,0}},
    {{0,0,0,1},{0,1,1,1}},
    {{1,1,0},{0,1,1}},
    {{0,1},{1,1},{1,0}}
};
int indice_peca = 0;
int coluna = 3; //Coluna no tabuleiro, posição horizontal
int linha = 0; // linha no tabuleiro, posição vertical
int pausado = 0;
/**
* Tela inicial do jogo em uma escala menor:
*/
int game_title[5][21] = {
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1}
};

/**
* Desenha a tela inicial do jogo em uma escala maior:
* Lógica de exibição:
* Como cada linha ocupa quatro blocos, então: linha = linha do "game_tile" + linha da tela + incremento de linha.
* Como cada coluna ocupa dois blocos, então: coluna = coluna do "game_tile" + coluna da tela + incremento de coluna.
* Ao fim de cada laço de linhas ou colunas na tela, deve-se incrementar as variáveis condicionais em 3 para linhas e 2 para colunas.
* Isso garante que cada linha ocupa quatro blocos e cada coluna ocupa dois blocos.
*/
void draw_start_screen(){
    int color;
    while(button() == 0){
        int choose_color = rand() % 2;
        apagar();
        background_color(COLOR_BLACK);
        int screen_row = 20;
        int screen_column = 9;
        for(int game_title_row = 0; game_title_row < 5; game_title_row++){
            for(int row_increment = 0; row_increment < 4; row_increment++){
                for(int game_title_column = 0; game_title_column < 21; game_title_column++){
                    for(int column_increment = 0; column_increment < 3; column_increment++){
                        if(game_title[game_title_row][game_title_column] == 1)
                            background_drawing(game_title_row+screen_row+row_increment, game_title_column+screen_column+column_increment, color);
                            buffer_overflow();
                    }
                    screen_column += 2;
                }
                screen_column = 9;  // Preparando a coluna inicial para a próxima linha;
            }
            screen_row += 3;
        }
        sleep(1);
        if(choose_color == 0)
            color = COLOR_BLUE;
        else if (choose_color == 1)
            color = COLOR_GREEN;
        else
            color = COLOR_MAGENTA;
    }
}

int desenharBloco(int coluna, int linha, int color){
    buffer_overflow();
    background_drawing(linha, coluna, color); 
    buffer_overflow();
    background_drawing(linha+1, coluna+1, color); 
    buffer_overflow();
    background_drawing(linha, coluna+1, color); 
    buffer_overflow();
    background_drawing(linha+1, coluna, color); 
}

int apagar_especifico(int linha, int coluna){
    buffer_overflow();
    background_block_erase(linha, coluna);
    return 0;
}

int apagar(){
      for (int coluna = 0; coluna < 80; coluna++){
        for (int linha = 0; linha < 60; linha ++){
            buffer_overflow();
            background_block_erase(linha, coluna); 
        }
    }

}

void desenharCampo(){
    //Limpa a tela primeiro
    apagar();
    
    //loop para  percorrer todas as linhas para desenhar as bordas laterais
    for(int y=0; y <= LINHA; y++){
        desenharBloco(0,y*2,COLOR_BLUE); //a borda esquerda
        desenharBloco((COLUNA+1)*2,y*2,COLOR_BLUE); // a borda direita 
    }   

    //percorre todas as colunas para desenhar a borda base
    for(int x=0;x<=COLUNA+1; x++){
        desenharBloco(x*2,LINHA*2,COLOR_BLUE); //Lembrando que LINHA é a ultima linha 
    }

    //laço para percorrer toda a matriz para fazer os blocos fixo nela
    for (int y=0; y< LINHA; y++){
        for (int x=0; x<COLUNA; x++){
            if(borda[y][x]){  // No caso, se tiver um valor positivo, ou seja, se tiver um bloco 
                //Verificando se no ponto atual na matriz o valor é 1, ou seja, tem peça
                desenharBloco((x+1)*2,y*2,COLOR_GRAY);
            }
            // Loop Infinito para Pausa:
            while(pausado){
                if(button() == 2)
                    pausado = 0; // Se "KEY0" for pressionado novamente, retoma o jogo.
            }
            if(button() == 1)
                pausado = 1; // KEY0 - Pausa o jogo
        }
    }

    //Desenha o bloco da peça na posição correta do tabuleiro, passando por toda a matriz do bloco
    for(int i=0; i<4;i++){
        for(int j=0; j<4;j++){
            if(pecas[indice_peca][i][j]){ //Se tiver um valor positivo, eu desenho
                //verificar que se na posição atual tem algum bloco, se sim ele precisa ser desenhado 
                desenharBloco((coluna+ j+1)*2, (linha + i)*2, COLOR_GREEN);
            }
        }
    
    
    }
}

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
                if (y + i >= LINHA|| x + j >= COLUNA || x + j< 0 || borda[y + i][x + j]) {
                    return 1;  // Retorna 1 se houver colisão
                }
            }
        }
    }
    return 0;  // Sem colisão
}
