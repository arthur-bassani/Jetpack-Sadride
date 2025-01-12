#include "raylib.h"

/* CONSTANTES */

// Configuracoes
#define FPS                60 // precisa? // sadride.h
#define RAND_SEED          time(0) // sadride.h?
#define VEL_MAPA_VARIAVEL  1
#define VER_TILES          0
#define VELOCIMETRO        1
//#define MAPA_PARADO          0 // temp
//#define TECLA_MOVER          KEY_A // temp
//#define TECLA_PARAR          KEY_P // nao pode existir no jogo final // temp
//#define TECLA_COLETAR_MOEDA  KEY_M // nao pode existir no jogo final // temp

// Maximo de itens em uma secao
#define MAX_ITENS LINHAS_SECAO * COLUNAS_SECAO

// Tamanho dos quadrados (importante para outras constantes) // sadride.h...
#define TAM_TILE 40

// Velocidade do mapa
#define VEL_INICIAL_MAPA   0.2000 * TAM_TILE
#define PASSO_VEL_MAPA     0.0004 * TAM_TILE // acho interessante que seja sutil, mas o valor pode 
                                             // depender da pontuacao para passar de fase...
#define VEL_MAX_MAPA       0.4000 * TAM_TILE

// Textos de pontuacao
#define TXT_PONT_FONTE  0.6 * TAM_TILE
#define TXT_PONT_COR    BLACK
#define TXT_PONT_X      0.1 * TAM_TILE
#define TXT_PONT_Y      1.1 * TAM_TILE
#define TXT_DIST_Y      0.1 * TAM_TILE + TXT_PONT_Y + TXT_PONT_FONTE
#define TXT_MOEDAS_Y    0.1 * TAM_TILE + TXT_DIST_Y + TXT_PONT_FONTE

/* ESTRUTURAS */

// Itens do jogo (paredes, moedas e espinhos)
typedef struct {
    char tipo;
    int x; // escolhi nao usar struct para a posicao
    int y;
} item_t;

/* FUNCOES (implementadas em fundo.c) */

// somente as que necessitam ser usadas fora de fundo.c... (acho que todas menos "main"...)
