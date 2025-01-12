//#include "mapas.h" // para as dimensoes do mapa
#include "fundo.h" // por causa de TAM_TILE

/* CONSTANTES */

#define FPS        60
#define RAND_SEED  time(0)

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
    SAIR
}EstadoJogo;

/* STRUCTS */

typedef struct leaderboard{ //leaderboard
    char nome[25];
    unsigned long int pontos;
}leaderboard_t;

/* FUNCOES */

//funcoes menu.c
int tela_inicial(); //menu inicial
int tela_leaderboard (int estado); //tela do leaderboard
int carregar_leaderboard(char *nome_arquivo, leaderboard_t *jogador); //retorna num de jogadores na tabela
void selection_sort(leaderboard_t *jogadores, int n_max); //ordena a exibicao na tela leaderboard

//funcoes main.c
void movimento(int *y, float *velocidade, int jetpackaltura); //vertical da jetpack
