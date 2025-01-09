typedef enum estados{  // Estados do Jogo
    MENU,
    JOGO,
    LEADERBOARD,
    SAIR
}EstadoJogo;

typedef struct leaderboard{ //leaderboard
    char nome[25];
    unsigned long int pontos;
}leaderboard_t;

//funcoes menu.c
int tela_inicial(); //menu inicial
int tela_leaderboard (int estado); //tela do leaderboard
int carregar_leaderboard(char *nome_arquivo, leaderboard_t *jogador); //retorna num de jogadores na tabela
void selection_sort(leaderboard_t *jogadores, int n_max); //ordena a exibicao na tela leaderboard

//funcoes main.c
void movimento(int *y, float *velocidade, int jetpackaltura); //vertical da jetpack

//constantes da janela 16:9 ratio
#define JANELA_Y 600
#define JANELA_X (JANELA_Y/9) *16

//constantes menu.c
#define BOTAO_LARG 200
#define BOTAO_ALT 50
