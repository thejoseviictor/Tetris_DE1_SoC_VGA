// Funções relacionadas aos componentes de entrada, ligados ao GPIO da DE1-SoC

#ifndef GPIO_H
#define GPIO_H

// Chamadas das Bibliotecas:
#include "jogabilidade.h"

// Cabeçalho das Funções:
int exibir_pontos(int pontos);
int button();
int* return_decomposto(int pontos);

#endif // GPIO_H
