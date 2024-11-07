.section .data
@Para virtualização de memória:
fd:                     .asciz "/dev/mem"
.EQU sys_open, 5
.EQU sys_close, 6
.EQU sys_exit, 1
.EQU sys_mmap2, 192
.EQU sys_munmap, 91
@Alguns foram dividos em MOVT (16 MSB bits) e MOVW (16 LSB bits),
@pois a instrução "MOV" pode receber o valor máximo de "255" (8 bits) na base 10
.EQU ALT_LWFPGASLVS_SPAN, 0x1000  @MOVW
.EQU ALT_LWFPGASLVS_OFST, 0xFF20  @MOVW
@Para barramento de dados:
.EQU DATA_A, 0x80
.EQU DATA_B, 0x70
.EQU RESET_PULSECOUNTER, 0x90
.EQU SCREEN, 0xA0
.EQU WRFULL, 0xB0
.EQU WRREG, 0xC0

.section .text
.global physical_to_virtual
.type	physical_to_virtual,%function

.global unmap_memory
.type	unmap_memory,%function

.global background_color
.type background_color,%function

.global background_drawing
.type background_drawing,%function

.global draw_polygon
.type draw_polygon,%function

.global set_sprite
.type set_sprite,%function

.global buffer_overflow
.type buffer_overflow,%function

.global error_open
.type error_mmap,%function

physical_to_virtual:
    PUSH {R1, R2, R3, R4, R5, R7}
    MOV R7, #sys_open
    LDR R0, =fd              @ Pathname = "/dev/mem"
    MOVW R1, #0x1002         @ Flags = O_RDWR | O_SYNC
    MOVT R1, #0x0010
    MOV R2, #0               @ Mode = 0
    SVC #0
    CMP R0, #0
    BLT error_open           @ Tratar erro se falhar ao abrir

    MOV R4, R0               @ Salva o descritor de arquivo
    MOV R7, #sys_mmap2
    MOV R0, #0               @ addr = NULL
    MOVW R1, #ALT_LWFPGASLVS_SPAN  @ length = 4096   
    MOV R2, #3               @ prot = PROT_READ | PROT_WRITE
    MOV R3, #1               @ flags = MAP_SHARED
    MOVW R5, #ALT_LWFPGASLVS_OFST  @ offset = 0xFF20
    SVC #0
    CMP R0, #0               @ Verifica se mapeamento foi bem-sucedido
    BLT error_mmap

    POP {R1, R2, R3, R4, R5, R7}
    BX LR                    @ Retorna da função

error_open:
    MOV R7, #sys_exit
    SVC #0

error_mmap:
    MOV R7, #sys_close
    MOV R0, R4
    SVC #0                   @ Fecha o descritor se mmap falhar
    MOV R7, #sys_exit
    SVC #0


@Função para remover o endereço mapeado da memória:
@Parâmetros:
@R0 = addr (endereço virtual)
@R1 = size_t (length)
unmap_memory:
    PUSH {R0, R1, R7}
    MOV R7, #sys_munmap
    SVC #0
    POP {R0, R1, R7}
    BX LR

@Função para mudar a cor do plano de fundo (Instrução WBR):
@Usa o Registrador R9 (1001)
@Parâmetros:
@R0 = Endereço virtual (Por enquanto, por causa do mapeamento em C)
@R1 = Cor
background_color:
    @Montando a instrução:
    MOV R9, #9
    LSL R9, #4
    @Enviado os dados da instrução:
    MOV R2, #0             @Salvado a constante "0" no R2
    MOV R3, #1             @Salvando a constante "1" no R3
    STR R2, [R0, #WRREG]   @Desativando o sinal de "start"
    STR R9, [R0, #DATA_A]  @opcode(0000) e registrador R9 (1001) para "DATA_A"
    STR R1, [R0, #DATA_B]  @Valor de cor do parâmetro, que foi armazenado em R1, para "DATA_B"
    STR R3, [R0, #WRREG]   @Sinal de "start"
    STR R2, [R0, #WRREG]   @Desativando o sinal de "start"
    BX LR

@Função para preencher áreas do background (Instrução WBM):
@Pequenos blocos de 8x8 pixels, com endereços de memória correspondes
@Quantidade de Blocos: 80 Horizontais e 60 Verticais
@Expressão do Endereço de 12 bits: (linha * 80) + coluna
@Parâmetros:
@R0 = Endereço virtual (Por enquanto, por causa do mapeamento em C)
@R1 = Coordenada da linha
@R2 = Coordenada da coluna
@R3 = Cor
background_drawing:
    @Calculando o endereço do bloco:
    MOV R4, #80              @Constante "80"
    MUL R1, R1, R4           @R1 = linha * 80
    ADD R1, R1, R2           @R1 = (linha * 80) + coluna
    @Montando a instrução:
    MOV R4, #2               @opcode(0010)
    LSL R1, #4               @Deslocando o R1 em 4 bits
    ADD R4, R4, R1           @Concatenando o "opcode" e o endereço de memória do bloco
    @Enviado os dados da instrução:
    MOV R5, #0               @Salvado a constante "0"
    MOV R6, #1               @Salvando a constante "1"
    STR R5, [R0, #WRREG]     @Desativando o sinal de "start"
    STR R4, [R0, #DATA_A]    @Enviando o "opcode" e o endereço de memória do bloco para o "DATA_A"
    STR R3, [R0, #DATA_B]    @Cor para "DATA_B"
    STR R6, [R0, #WRREG]     @Sinal de "start"
    STR R5, [R0, #WRREG]     @Desativando o sinal de "start"
    BX LR
    @ Ideia: remover o mapeamento aqui

@Função para desenhar um polígono (Instrução DP):
@Parâmetros:
@R0 = Endereço virtual (Por enquanto, por causa do mapeamento em C)
@R1 = Coordenada de Referência X (Máximo 512)
@R2 = Coordenada de Referência Y (Máximo 512)
@R3 = Tamanho
@R4 = Cor
@R5 = Tipo: Quadrado ou Triângulo
draw_polygon:
    @Montando a instrução:
    MOV R7, #3              @opcode(0011)
    LSL R2, R2, #9          @Deslocando o R3 (y) em 9 bits
    LSL R3, R3, #18         @Deslocando o R4 (tamanho) em 18 bits
    LSL R4, R4, #22         @Deslocando o R5 (cor) em 22 bits
    LSL R5, R5, #31         @Deslocando o R6 (tipo) em 31 bits
    ADD R1, R1, R2          @Concatenando x e y
    ADD R1, R1, R3          @Concatenando x, y e tamanho
    ADD R1, R1, R4          @Concatenando x, y, tamanho e cor
    ADD R1, R1, R5          @Concatenando x, y, tamanho, cor e tipo
    @Enviado os dados da instrução:
    MOV R2, #0              @Salvado a constante "0"
    MOV R3, #1              @Salvando a constante "1"
    STR R2, [R0, #WRREG]    @Desativando o sinal de start
    STR R7, [R0, #DATA_A]   @"opcode" e "endereço" para "DATA_A"
    STR R1, [R0, #DATA_B]   @"x", "y", "tamanho", "cor" e "tipo" para "DATA_B"
    STR R3, [R0, #WRREG]    @Sinal de start
    STR R2, [R0, #WRREG]    @Desativando o sinal de start
    BX LR

@Função para configurar um sprite (Instrução WBR):
@Parâmetros:
@R0 = Endereço virtual (Por enquanto, por causa do mapeamento em C)
@R1 = Registrador R0-R30 (00000 - 11110)
@R2 = Offset (9 bits) (Máximo: 511)
@R3 = Coordenada x (10 bits) (Máximo: 1023)
@R4 = Coordenada y (10 COLOR_BLUEbits) (Máximo: 1023)
@R5 = sp (0 - Desabilitado; 1 - Habilitado)
set_sprite:
    @Montando a instrução:
    LSL R1, #4             @Deslocando "registrador" em 4 bits
    LSL R3, #19            @Deslocando o "x" em 9 bits
    LSL R4, #9             @Deslocando o "y" em 19 bits
    LSL R5, #29            @Deslocando o "sp" em 29 bits
    ADD R2, R3             @Concatenando "offset" e "x"
    ADD R2, R4             @Concatenando "offset", "x" e "y"
    ADD R2, R5             @Concatenando "offset", "x", "y" e "sp"
    @Enviado os dados da instrução:
    MOV R3, #0             @Salvado a constante "0"
    MOV R4, #1             @Salvando a constante "1"
    STR R3, [R0, #WRREG]   @Desativando o sinal de "start"
    STR R1, [R0, #DATA_A]  @"opcode" e "registrador" para "DATA_A"
    STR R2, [R0, #DATA_B]  @"offset", "x", "y", "sp"
    STR R4, [R0, #WRREG]   @Sinal de "start"
    STR R3, [R0, #WRREG]   @Desativando o sinal de "start"
    BX LR

@Desvio que previne o overflow do buffer de "DATA_A" e "DATA_B":
@Parâmetros:
@R0: Endereço Virtual
buffer_overflow:
    LDR R1, [R0, #WRFULL]
    CMP R1, #1
    BEQ buffer_overflow
    BX LR
