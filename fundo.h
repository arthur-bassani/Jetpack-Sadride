#include "raylib.h"

/* CONSTANTES */

// Apenas para desenvolvimento
#define VER_TILES            0
#define MAPA_PARADO          0
#define TECLA_MOVER          KEY_A
#define TECLA_COLETAR_MOEDA  KEY_M

// Configuracoes
#define FPS 60 // precisa?
#define RAND_SEED time(0)

// Maximo de itens em uma secao
#define MAX_ITENS LINHAS_SECAO * COLUNAS_SECAO

// Tamanho dos quadrados (importante para outras constantes)
#define TAM_TILE 40

// Velocidade dos itens
#define VEL_INICIAL_MAPA 0.20 * TAM_TILE
//#define PASSO_VEL_MAPA 0 // ? // Velocidade variavel nao eh requisito!
//#define VEL_MAX_MAPA 40 // ?

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
    int x; // nao quero lidar com structs aninhadas aqui... ao menos por enquanto
    int y;
} item_t;

/* FUNCOES (implementadas em fundo.c) */

// somente as que necessitam ser usadas fora de fundo.c...
