// Funções para Acessar e Manipular a Memória e o Acelerômetro ADXL345 na DE1-SoC

#ifndef ADXL345_H
#define ADXL345_H

// Chamadas das Bibliotecas:
#include "jogabilidade.h"

// Registradores do ADXL345:
#define ADXL345_ADDRESS 0x53 // Endereço Base do Acelerômetro
#define THRESH_ACT 0x24
#define THRESH_INACT 0x25
#define TIME_INACT 0x26
#define ACT_INACT_CTL 0x27
#define BW_RATE 0x2C
#define POWER_CTL 0x2D
#define INT_ENABLE 0x2E
#define DATA_FORMAT 0x31
#define DATAX0 0x32 // Byte Menos Significativo de X
#define DATAX1 0x33 // Byte Mais Significativo de X

// Variáveis que serão usadas em outras bibliotecas:
extern int fd; // "fd": file descriptor;

// Cabeçalho das Funções:
void open_memory(void);
void I2C0_mapping(void);
void enable_I2C0_HPS(void);
void setting_I2C0_ADXL345(void);
void setting_ADXL345(void);
int16_t lerEixoX(void);

#endif // ADXL345_H
