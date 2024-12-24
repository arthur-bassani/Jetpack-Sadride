#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "sadride.h" //comentar caso nao compile

int tela_inicial(){
    InitWindow(JANELA_X, JANELA_Y, "Jetpack Sadride");
    SetTargetFPS(60);
    //variaveis do menu gamer
    EstadoJogo estado = MENU;
    // Rectangle é uma struct da raylib mesmo
    Rectangle botaoNovoJogo = {JANELA_X/2 - BOTAO_LARG/2 , JANELA_Y /2 - 25, BOTAO_LARG, BOTAO_ALT};  // x, y, largura e altura
    Rectangle botaoLeaderboard = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y /2 + 50, BOTAO_LARG, BOTAO_ALT};
    Rectangle botaoSair = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y/2 + 125, BOTAO_LARG, BOTAO_ALT};

    EnableCursor();
    ShowCursor();

    char titulo[] = "Jetpack Sadride Demo";

    while (!WindowShouldClose()){
        Vector2 mousePos = GetMousePosition(); // Vector2 é struct que pega x e y (propria raylib)

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

        DrawText(titulo, JANELA_X / 2 - (MeasureText(titulo, 26)/2), JANELA_Y/2 - 100, 26, WHITE);
        // centralizar texto: ( tamanho tela / 2) - (tamanho texto / 2)

        //botoes
        DrawRectangleRec(botaoNovoJogo, LIGHTGRAY);
        DrawText("Novo Jogo", botaoNovoJogo.x + 50, botaoNovoJogo.y + 15, 20, BLACK);

        DrawRectangleRec(botaoLeaderboard, LIGHTGRAY);
        DrawText("Leaderboard", botaoLeaderboard.x + 35, botaoLeaderboard.y + 15, 20 ,BLACK);

        DrawRectangleRec(botaoSair, LIGHTGRAY);
        DrawText("Sair", botaoSair.x + 80, botaoSair.y + 15, 20, BLACK);

        //Verificação dos botoes
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, botaoNovoJogo)) {
                estado = JOGO;
                break;
            } else if (CheckCollisionPointRec(mousePos, botaoLeaderboard)) {
                estado = LEADERBOARD;
                estado = tela_leaderboard (estado);
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

int tela_leaderboard (int estado){

    SetTargetFPS(60);
    leaderboard_t jogador[5]; //num de jogadores q podem aparecer na tela

    while(estado == LEADERBOARD && !WindowShouldClose()){
        Vector2 mousePos = GetMousePosition(); // Vector2 é struct que pega x e y (propria raylib)
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

        DrawText("Leaderboard", JANELA_X / 2 - (MeasureText("Leaderboard", 30)/2), 50, 30, WHITE);
        
        Rectangle botaoRetornar = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y-100, BOTAO_LARG, BOTAO_ALT};
        DrawRectangleRec(botaoRetornar, LIGHTGRAY);
        DrawText("Retornar", JANELA_X / 2  - (MeasureText("Retornar", 20)/2), botaoRetornar.y + 15, 20, BLACK);

        int num_jogadores_leaderboard = carregar_leaderboard("leaderboard.csv", jogador); //num de jogadores que aparecerao 

        if(num_jogadores_leaderboard == -1){ DrawText("Erro ao carregar Leaderboard!",
                        JANELA_X / 2 - (MeasureText("Erro ao carregar Leaderboard!", 26)/2),JANELA_Y/2, 26, RED);}
        else if(num_jogadores_leaderboard == 0) {DrawText("Sem jogadores no Leaderboard ainda!",
                        JANELA_X / 2 - (MeasureText("Sem jogadores no Leaderboard ainda!", 26)/2), JANELA_Y/2, 26, YELLOW);}
        else{
            for (int i = 0; i < num_jogadores_leaderboard; i++) {
                char buffer[100]; //str que aparecera na tela
                snprintf(buffer, sizeof(buffer), "%d. %s - %d", i + 1, jogador[i].nome, jogador[i].pontos);
                Color cor;
                switch (i){
                case 0: cor = GOLD; break;
                case 1: cor = LIGHTGRAY; break;
                case 2: cor = BROWN; break;
                default: cor = DARKGRAY; }
                DrawText(buffer, JANELA_X/2 - MeasureText(buffer, 20)/2, 120 + i * 40, 20, cor); // Posição dinâmica para cada linha
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, botaoRetornar)) {
                estado = MENU;
            }
        }
        EndDrawing();
    }
    return estado;
}


int carregar_leaderboard(char *filename, leaderboard_t *jogador){

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return -1;

    char linha[200]; //str temporaria p/ armazenar uma linha do csv
    int i = 0;

     // Lendo o arquivo linha por linha
    while (fgets(linha, sizeof(linha), fp)) {
        char *token;
        // Extraindo o nome
        token = strtok(linha, ",");
        if (token != NULL) {
            strncpy(jogador[i].nome, token, sizeof(jogador[i].nome));
            jogador[i].nome[sizeof(jogador[i].nome) - 1] = '\0'; // Garantir que a string termine com '\0'
        }
        // Extraindo os pontos
        token = strtok(NULL, ",");
        if (token != NULL) {
            jogador[i].pontos = atoi(token);
        }
        i++;
    }

    fclose(fp);
    return i;
}