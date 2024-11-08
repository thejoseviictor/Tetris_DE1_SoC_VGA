// Função que inicializa o jogo Tetris do Problema 02

#include "jogabilidade.h"

int main(){
    physical_to_virtual();  // Mapeando os Endereços do VGA
    open_memory();
    GPIO_mapping();
    I2C0_mapping();
    enable_I2C0_HPS();
    setting_I2C0_ADXL345(); // Configurando o Barramento I2C0
    setting_ADXL345();      // Configurando os Sensores do Acelerômetro
    draw_start_screen();    // Desenha a tela inicial do jogo
    int16_t eixo_x; 
    int in_game = 1;
    int playing = 1;
    while(in_game){
        while(playing){
            eixo_x = lerEixoX();
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
                    for(int row = 0; row < LINHA; row++){
                        for(int column = 0; column < COLUNA; column++){
                            borda[row][column] = 0;
                        }
                    }
                    draw_start_screen();
                    playing = 0; // Fim de Jogo
                }
            }
            desenharCampo();
            usleep(300 * 1000); // Controla a velocidade da queda
            exibir_pontos(pontuacao);
        }
        // Reinicia o jogo ao apertar o KEY0
        if(button() == 1){
            pontuacao = 0;
            playing = 1;
        }
        // Finaliza o jogo ao apertar o KEY3
        else if(button() == 4)
            in_game = 0;
    }
    unmap_memory();
    return 0;
}
