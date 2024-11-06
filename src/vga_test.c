#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h> // Entrada e Saída, como "printf"
#include <stdlib.h>
#include "fcntl.h" // Para Manipulação de Arquivos
#include "unistd.h" // Para Fechar Arquivos
#include "time.h" // Função "sleep"
#include "sys/mman.h"// Para Mapear Endereço Físico em Endereço Virtual
#include <time.h>

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

// Instruções do VGA:
extern void physical_to_virtual();                 // Syscalls "open" e "mmap2"
extern void unmap_memory();                        // Syscall "munmap"
extern void background_color(int);                 // Instrução WBR;
extern void background_drawing(int, int, int);     // Instrução WBM;
extern void buffer_overflow();                     // Instrução WBM;
extern void draw_polygon(int, int, int, int, int); // Instrução DP;
extern void set_sprite(int, int, int, int);        // Instrução WBR;

int main(){
    physical_to_virtual();
    background_color(COLOR_BLUE);
    background_drawing(30, 40, COLOR_GREEN);
    return 0;
}
