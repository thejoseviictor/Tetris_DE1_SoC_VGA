#include "jogabilidade.h"

int main(){
    physical_to_virtual();
    for(int linha = 0; linha < 60; linha++){
        for(int coluna = 0; coluna < 80; coluna++){
            buffer_overflow();
            background_block_erase(linha, coluna);
        }
    }
    background_color(COLOR_WHITE);
    for(int linha = 0; linha < 10; linha++){
        buffer_overflow();
        background_drawing(linha, 0, COLOR_RED);
        usleep(250 * 1000);
        background_block_erase(linha, 0);
    }
    draw_polygon(20, 5, 20, 0, COLOR_BLUE, 0);
    draw_polygon(22, 15, 50, 0, COLOR_GREEN, 1);
    unmap_memory();
    return 0;
}