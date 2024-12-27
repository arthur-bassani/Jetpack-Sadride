/* CONSTANTES */

// Apenas para desenvolvimento
#define VER_TILES    0
#define MAPA_PARADO  0
#define TECLA_MOVER  KEY_A

// Configuracoes
#define FPS 60 // precisa?
#define RAND_SEED time(0)

// Maximo de itens em uma secao
#define MAX_ITENS LINHAS_SECAO * COLUNAS_SECAO

// Tamanho dos quadrados (importante para outras constantes)
// nao funciona com alguns valores, por conta da relacao com a velocidade...
// 20, 40 e 60 funcionam... (com velocidade inicial 8, pelo menos)
#define TAM_TILE 40

// Velocidade dos itens
#define VEL_INICIAL_MAPA 8 // deixar em funcao de TAM_TILE?
//#define PASSO_VEL_MAPA 1 // ?
//#define VEL_MAX_MAPA 40 // ?

// Texto "Distancia percorrida"
#define TXT_DIST_X      0.1 * TAM_TILE
#define TXT_DIST_Y      1.1 * TAM_TILE
#define TXT_DIST_FONTE  0.6 * TAM_TILE
#define TXT_DIST_COR    BLACK

// Texto "Moedas coletadas"
#define TXT_MOEDAS_X      TXT_DIST_X
#define TXT_MOEDAS_Y      0.1 * TAM_TILE + TXT_DIST_Y + TXT_DIST_FONTE
#define TXT_MOEDAS_FONTE  TXT_DIST_FONTE
#define TXT_MOEDAS_COR    BLACK

/* ESTRUTURAS */

// Itens do jogo (paredes, moedas e espinhos)
typedef struct {
    char tipo;
    int x; // nao quero lidar com structs aninhadas aqui... ao menos por enquanto
    int y;
} item_t;

/* FUNCOES (implementadas em fundo.c) */

// somente as que necessitam ser usadas fora de fundo.c...
