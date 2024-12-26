#include "raylib.h"
#include "sadride.h"

typedef struct{
    int x, y;
    float velocidade;
    int largura, altura;
}personagem_t;

void movimento(int *y, float *velocidade, int jetpackaltura) {
    const float gravidade = 0.5f; //valores de gravidade e impulso livres para mudanca
    const float impulso = -5.0f;

    // veloc afetada por impulso caso ESPACO
    if (IsKeyDown(KEY_SPACE)) {
        *velocidade = impulso;
    } else {
        *velocidade += gravidade; // se nao, veloc afetada por gravidade
    }

    //atualiza a posicao (precisa ser int)
    *y += (int)*velocidade;

    // previne que suba da janela
    if (*y < 0) {
        *y = 0;
        *velocidade = 0; // para o movimento
    }

    // previne que caia da janela
    int groundLevel = JANELA_Y - jetpackaltura;
    if (*y > groundLevel) {
        *y = groundLevel;
        *velocidade = 0; // para o movimento
    }
}

int main(){
    EstadoJogo estado = MENU;
    while(estado != SAIR){
        
        bool isPaused = false;      // variavel de controle para pausa
        if(estado == MENU) estado = tela_inicial();
        if (estado == JOGO) {  // verificar se o jogo deve ser rodado
            InitWindow(JANELA_X, JANELA_Y, "Jetpack Sadride");
            SetExitKey(0); // O ESC nao fecha mais a janela
            DisableCursor(); HideCursor();

            personagem_t jetpack;
            jetpack.x = JANELA_X/6;
            jetpack.y = JANELA_Y;
            jetpack.velocidade = 0;
            //interessante deixar tamanho do jetpack como variavel caso tenhamos um powerup
            jetpack.largura = 30;
            jetpack.altura = 50;

            SetTargetFPS(60); // fps do jogo

            // Main game loop
            while (estado == JOGO && !WindowShouldClose()){
                DrawFPS(10,10);
                if(IsKeyPressed(KEY_ESCAPE)) {  // mudar facilmente o estado de pausa
                    isPaused = !isPaused;
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);

                if (!isPaused) { // quando está rodando
                    movimento(&jetpack.y, &jetpack.velocidade, jetpack.altura);
                    //DrawRectangle(jetpack.x, jetpack.y, jetpack.largura, jetpack.altura, BLACK);
                }

                DrawRectangle(jetpack.x, jetpack.y, jetpack.largura, jetpack.altura, BLACK); // Ao apertar ESC nao vai sumir o personagem

                // jogo pausado
                if (isPaused) {  // Measure Text pra ficar centralizado bem bunitinhu
                    DrawText("Jogo Pausado", JANELA_X/2 - MeasureText("Jogo Pausado", 30)/2, JANELA_Y/2 - 20, 30, WHITE);
                    DrawText("Pressione ESC para continuar ou M para retornar ao menu",
                        JANELA_X/2 - MeasureText("Pressione ESC para continuar ou M para retornar ao menu", 20)/2,
                        JANELA_Y/2 + 20, 20, GRAY);
                    if(IsKeyPressed(KEY_M)) {estado = MENU;}
                }

                EndDrawing();
            }
            CloseWindow();
        }
    }
    CloseWindow();
    return 0;
}
