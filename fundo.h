#include "mapas.h"

/* CONSTANTES */

// Tamanho dos quadrados (importante para outras constantes)
#define TAM_TILE 40 // precisa ser par

// Porcentagem do diametro da moeda em relacao a TAM_TILE (deve ser menor que 1)
#define PC_DIAM_MOEDA 0.9

// Configuracoes
#define VEL_MAPA_VARIAVEL  1
#define VER_TILES          0
#define VELOCIMETRO        0

// Maximo de itens em uma secao
#define MAX_ITENS LINHAS_SECAO * COLUNAS_SECAO

// Velocidade do mapa
#define VEL_INICIAL_MAPA  0.2000 * TAM_TILE
#define PASSO_VEL_MAPA    0.0001 * TAM_TILE // acho interessante que seja sutil
#define VEL_MAX_MAPA      0.4000 * TAM_TILE

/* ESTRUTURAS */

// Itens do jogo (paredes, moedas e espinhos)
typedef struct {
    char tipo;
    int x; // escolhi nao usar struct para a posicao
    int y;
} item_t;

// Circulo para representar as moedas, tanto na hora de desenhar quanto ver as colisoes
typedef struct {
    Vector2 centro;
    int raio; // soh usamos int
} circulo_t;

/* FUNCOES (implementadas em fundo.c) */

int inicio_secao_aleatorio();

void gerar_secao(item_t itens[MAX_ITENS], char mapa[LINHAS_MAPA][COLUNAS_MAPA], int j_inicial_mapa, int j_offset_tela);

Rectangle item_retangulo(item_t *item);

circulo_t item_circulo(item_t *item);

void desenhar_item(item_t *item);

int item_passou_inicio_tela(int x_antes, int x_agora);

void aumentar_velocidade(float *velocidade_mapa, float acrescimo, float maximo);

void deslizamento_secoes(item_t atual[], item_t proxima[], char mapa[LINHAS_MAPA][COLUNAS_MAPA]);
