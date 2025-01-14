#include "raylib.h"
#include "fundo.h" // por causa de TAM_TILE

/* CONSTANTES */
#define MAX_CHEAT_LENGTH 10
#define CHEAT_CODE "lucas"

#define FPS        60
#define RAND_SEED  time(0)

// Pontuacoes para passar de fase
#define PONT_FASE_2 3000
#define PONT_FASE_3 7500

// Textos de pontuacao
#define TXT_PONT_FONTE  0.8 * TAM_TILE
#define TXT_PONT_COR    WHITE
#define TXT_PONT_X      0.1 * TAM_TILE
#define TXT_PONT_Y      1.1 * TAM_TILE
#define TXT_DIST_Y      0.1 * TAM_TILE + TXT_PONT_Y + TXT_PONT_FONTE
#define TXT_MOEDAS_Y    0.1 * TAM_TILE + TXT_DIST_Y + TXT_PONT_FONTE

// Missil
#define DIST_MIN_MISSIL    50   // distancia minima a ser percorrida para o primeiro missil aparecer
#define FATOR_FREQ_MISSIL  70   // pareceu bom, mas tem que ver
#define FATOR_VEL_MISSIL   1.75 // velocidade do missil em relacao a do mapa
#define COR_MISSIL         RED
#define COR_INV_GRAV       BLUE

// Laser
//#define COR_ARMADO     ORANGE
//#define COR_DISPARADO  RED

//constantes da janela 16:9 ratio
//#define JANELA_Y 600
//#define JANELA_X (JANELA_Y/9) *16

// constantes da janela usando o tamanho dos quadrados (para os menus pode ser diferente...)
#define JANELA_Y  LINHAS_SECAO * TAM_TILE
#define JANELA_X  COLUNAS_SECAO * TAM_TILE

//constantes menu.c
// interessante deixar em funcao de TAM_TILE ou JANELA_X/Y...
#define BOTAO_LARG 200
#define BOTAO_ALT 50

/* ENUMS */

typedef enum estados{  // Estados do Jogo
    MENU,
    JOGO,
    LEADERBOARD,
    SAIR,
    GAMEOVER
}EstadoJogo;

/* STRUCTS */

typedef struct leaderboard{ //leaderboard
    char nome[20];
    int pontos;
}leaderboard_t;

/* FUNCOES */

//funcoes menu.c
int tela_inicial(); //menu inicial
int tela_leaderboard (int estado); //tela do leaderboard
int carregar_leaderboard(char *nome_arquivo, leaderboard_t *jogador); //retorna num de jogadores na tabela
void selection_sort(leaderboard_t *jogadores, int n_max); //ordena a exibicao na tela leaderboard
int atualizar_leaderboard(char *nome_arquivo, int pontuacao, int num_jogadores_leaderboard); //atualiza leaderboard apos gameover
int tela_gameover(int estado, int pontuacao); //tela do gameover

//funcoes main.c
void movimento(int *y, float *velocidade, int jetpackaltura); //vertical da jetpack
