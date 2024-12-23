#include "raylib.h"

/*
int main(){ //funcao main para testar menu.c | apagar depois!
    EstadoJogo estado = tela_inicial();

    if (estado == JOGO) {   // nova janela para o jogo em si      FAZER ASSIM?
        InitWindow(JANELA_X, JANELA_Y, "Gameplay Insana de League");
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Iniciando Juego... ", JANELA_X/2 - 100 ,JANELA_Y/2, 20, WHITE);
            DrawText("Aqui que eh pra comecar o jogo", JANELA_X/2 -175, JANELA_Y/2 +50, 20, WHITE);
            EndDrawing();
        }
        CloseWindow();
    }

    return 0;
} */

EstadoJogo tela_inicial(){
    InitWindow(JANELA_X, JANELA_Y, "Jetpack Sadride");

    //variaveis do menu gamer
    EstadoJogo estado = MENU;   // Rectangle é uma struct da raylib mesmo
    Rectangle botaoNovoJogo = {JANELA_X/2 - BOTAO_LARG/2 , JANELA_Y /2 - 25, BOTAO_LARG, BOTAO_ALT};  // x, y, largura e altura
    Rectangle botaoLeaderboard = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y /2 + 50, BOTAO_LARG, BOTAO_ALT};
    Rectangle botaoSair = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y/2 + 125, BOTAO_LARG, BOTAO_ALT};

    EnableCursor();
    ShowCursor();

    char titulo[]= "Jetpack Sadride Demo";

    while (!WindowShouldClose()){
        Vector2 mousePos = GetMousePosition(); // Vector2 é struct que pega x e y (propria raylib)
        // A função ali tambem facilita nossa vida, o nome dela diz tudo

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

        DrawText(titulo, JANELA_X / 2 - (MeasureText(titulo, 26)/2), JANELA_Y/2 - 100, 26, WHITE);
        //formula para centralizar texto: (metade da tela / 2) - (tamanho texto / 2)

        //butoes
        DrawRectangleRec(botaoNovoJogo, LIGHTGRAY);
        DrawText("Novo Jogo", botaoNovoJogo.x + 50, botaoNovoJogo.y + 15, 20, BLACK);

        DrawRectangleRec(botaoLeaderboard, LIGHTGRAY);
        DrawText("Leaderboard", botaoLeaderboard.x + 35, botaoLeaderboard.y + 15, 20 ,BLACK);

        DrawRectangleRec(botaoSair, LIGHTGRAY);
        DrawText("Sair", botaoSair.x + 70, botaoSair.y + 15, 20, BLACK);

        //Verificação dos botoes
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, botaoNovoJogo)) {
                estado = JOGO;
                break; //PRECISA?
            } else if (CheckCollisionPointRec(mousePos, botaoLeaderboard)) {
                estado = LEADERBOARD;
                break;
            } else if (CheckCollisionPointRec(mousePos, botaoSair)) {
                estado = SAIR;
                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return estado;
}
