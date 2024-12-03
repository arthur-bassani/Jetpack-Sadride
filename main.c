
#include "raylib.h"

typedef struct  {
    int x, y;
    float velocidade;
    int largura, altura;
}Personagem_t;

void Movimento (int *y, float *velocidade, int jetpackaltura) {
    
    if(*y < 450){
    float aceleracao = 0.5;
    if (IsKeyDown(KEY_SPACE)) {
        *velocidade = -5;
    }else {
        *velocidade += aceleracao;
    }
    if(*y += *velocidade <= 450 - jetpackaltura){
        *y += *velocidade;
    }else{
        *y = 450 - jetpackaltura;   
    }
    }else{
    *y = 450 - jetpackaltura;
    }
}

int main()
{

    InitWindow(800, 450, "Jetpack Sadride");

    Personagem_t jetpack;
    jetpack.x = 400;
    jetpack.y = 225;
    jetpack.velocidade = 0;
    jetpack.largura = 30;
    jetpack.altura = 50;

    SetTargetFPS(60); // fps do jogo

    // Main game loop
    while (!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        Movimento (&jetpack.y, &jetpack.velocidade, jetpack.altura);
        
        DrawRectangle(jetpack.x, jetpack.y, jetpack.largura, jetpack.altura, BLACK);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
