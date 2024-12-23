typedef enum {  // Estados do Jogo
    MENU,
    JOGO,
    LEADERBOARD,
    SAIR
}EstadoJogo;

EstadoJogo tela_inicial();
void movimento(int *y, float *velocidade, int jetpackaltura);

//constantes da janela
#define JANELA_Y 450
#define JANELA_X 800
#define BOTAO_LARG 200
#define BOTAO_ALT 50
