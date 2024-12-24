typedef enum estados{  // Estados do Jogo
    MENU,
    JOGO,
    LEADERBOARD,
    SAIR
}EstadoJogo;

typedef struct leaderboard{ //leaderboard
    char nome[20];
    int pontos;
}leaderboard_t;

//funcoes menu.c
int tela_inicial(); //menu inicial
int tela_leaderboard (int estado); //tela do leaderboard
int carregar_leaderboard(char *filename, leaderboard_t *jogador); //extrai conteudo do csv para leaderboard_t jogador

//funcoes main.c
void movimento(int *y, float *velocidade, int jetpackaltura); //vertical da jetpack

//constantes da janela
#define JANELA_Y 450
#define JANELA_X 800

//constantes dos botoes menu.c
#define BOTAO_LARG 200
#define BOTAO_ALT 50

//files .c para que nao seja preciso compilar outros files junto com main.c
//deixa-los comentados /* */  caso compile junto
#include "menu.c"