#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sadride.h"

int tela_inicial(){

    //variaveis do menu gamer
    EstadoJogo estado = MENU;
    // Rectangle é uma struct da raylib mesmo
    Rectangle botaoNovoJogo = {JANELA_X/2 - BOTAO_LARG/2 , JANELA_Y /2 - 25, BOTAO_LARG, BOTAO_ALT};  // x, y, largura e altura
    Rectangle botaoLeaderboard = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y /2 + 50, BOTAO_LARG, BOTAO_ALT};
    Rectangle botaoSair = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y/2 + 125, BOTAO_LARG, BOTAO_ALT};

    Rectangle botaoTeclaN = {JANELA_X/2 - BOTAO_LARG/2 - 70, JANELA_Y /2 - 25, BOTAO_ALT, BOTAO_ALT};
    Rectangle botaoTeclaJ = {JANELA_X/2 - BOTAO_LARG/2 - 70, JANELA_Y /2 + 50, BOTAO_ALT, BOTAO_ALT};
    Rectangle botaoTeclaQ = {JANELA_X/2 - BOTAO_LARG/2 - 70, JANELA_Y /2 + 125, BOTAO_ALT, BOTAO_ALT};

    EnableCursor();
    ShowCursor();

    char titulo[] = "Jetpack Sadride";

    while (!WindowShouldClose()){
        Vector2 mousePos = GetMousePosition(); // Vector2 é struct que pega x e y (propria raylib)

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

        DrawText(titulo, JANELA_X / 2 - (MeasureText(titulo, 40)/2), JANELA_Y/2 - 150, 40, WHITE);
        // centralizar texto: ( tamanho tela / 2) - (tamanho texto / 2)

        //botoes
        DrawRectangleRec(botaoNovoJogo, LIGHTGRAY);
        DrawText("Novo Jogo", botaoNovoJogo.x + 50, botaoNovoJogo.y + 15, 20, BLACK);
        DrawRectangleRec(botaoTeclaN, GRAY);
        DrawText("N", botaoTeclaN.x +10, botaoTeclaN.y +2, 50, BLACK);

        DrawRectangleRec(botaoLeaderboard, LIGHTGRAY);
        DrawText("Leaderboard", botaoLeaderboard.x + 35, botaoLeaderboard.y + 15, 20 ,BLACK);
        DrawRectangleRec(botaoTeclaJ, GRAY);
        DrawText("J", botaoTeclaJ.x +10, botaoTeclaJ.y +2, 50, BLACK);


        DrawRectangleRec(botaoSair, LIGHTGRAY);
        DrawText("Sair", botaoSair.x + 80, botaoSair.y + 15, 20, BLACK);
        DrawRectangleRec(botaoTeclaQ, GRAY);
        DrawText("Q", botaoTeclaQ.x +10, botaoTeclaQ.y +2, 50, BLACK);

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
        //if(IsKeyPressed(KEY_O)) {estado= GAMEOVER; estado = tela_gameover(estado, 999999);} //para testar tela gameover
        if(IsKeyPressed(KEY_N)) {estado = JOGO; break;}
        if(IsKeyPressed(KEY_J)) {estado = LEADERBOARD; estado = tela_leaderboard(estado);}
        if(IsKeyPressed(KEY_Q)) {estado = SAIR; break;}

        EndDrawing();
    }
    return estado;
}

int tela_leaderboard (int estado){

    int i;
    SetTargetFPS(FPS);
    leaderboard_t jogador[5]; //num de jogadores q podem aparecer na tela
    Rectangle botaoRetornar = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y-100, BOTAO_LARG, BOTAO_ALT};
    Rectangle botaoTeclaM = {JANELA_X/2 - BOTAO_LARG/2 -70, JANELA_Y-100, BOTAO_ALT, BOTAO_ALT};

    unsigned int num_jogadores_leaderboard = carregar_leaderboard("leaderboard.bin", jogador); 
    selection_sort(jogador, num_jogadores_leaderboard);

    while(estado == LEADERBOARD && !WindowShouldClose()){
        Vector2 mousePos = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

        DrawText("Leaderboard", JANELA_X / 2 - (MeasureText("Leaderboard", 30)/2), 50, 30, WHITE);
        
        DrawRectangleRec(botaoRetornar, LIGHTGRAY);
        DrawText("Retornar", JANELA_X / 2  - (MeasureText("Retornar", 20)/2), botaoRetornar.y + 15, 20, BLACK);
        DrawRectangleRec(botaoTeclaM, GRAY);
        DrawText("M", botaoTeclaM.x + 8, botaoTeclaM.y + 2, 50, BLACK);

        if(num_jogadores_leaderboard == -1){ DrawText("Erro ao carregar Leaderboard!",
                        JANELA_X / 2 - (MeasureText("Erro ao carregar Leaderboard!", 26)/2),JANELA_Y/2, 26, RED);}
       // else if(num_jogadores_leaderboard == 0) {DrawText("Sem jogadores no Leaderboard ainda!",
       //                 JANELA_X / 2 - (MeasureText("Sem jogadores no Leaderboard ainda!", 26)/2), JANELA_Y/2, 26, YELLOW);}
       //criar um arquivo.bin ja com 5 jogadores como easter egg? facilita nossa vida alem de ser lgl
        else{
            for (i = 0; i < num_jogadores_leaderboard; i++) {
                char str_temp[100]; //string que aparecera na tela
                snprintf(str_temp, 100, "%d. %s - %d", i + 1, jogador[i].nome, jogador[i].pontos);
                Color cor;
                switch (i){
                case 0: cor = GOLD; break;
                case 1: cor = LIGHTGRAY; break;
                case 2: cor = BROWN; break;
                default: cor = DARKGRAY; }
                DrawText(str_temp, JANELA_X/2 - MeasureText(str_temp, 20)/2, JANELA_Y/3 + i * 40, 20, cor);
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, botaoRetornar)) {
                estado = MENU;
            }
        }
        if(IsKeyPressed(KEY_M)) {estado = MENU;}
        
        EndDrawing();
    }
    return estado;
}


int carregar_leaderboard(char *nome_arquivo, leaderboard_t *jogador){ //retorna -1 caso de erro

    FILE *fp = fopen(nome_arquivo, "rb");
    if(fp == NULL) return -1;
    
    unsigned int i = fread(jogador, sizeof(leaderboard_t), 5, fp);

    fclose(fp);
    return i; //sempre retorna 5 
}

void selection_sort(leaderboard_t *jogadores, int n_max) { //ordena a exibicao na tela leaderboard
    int i, j, maior_i;

    for (i = 0; i < n_max - 1; i++) {
        maior_i = i; // Assume que o maior elemento está na posição atual

        for (j = i + 1; j < n_max; j++) {
            if (jogadores[j].pontos > jogadores[maior_i].pontos) {
                maior_i = j; // Atualiza o indice do maior elemento encontrado
            }
        }
        // Trocar jogadores[i] e jogadores[maior_i]
        if (maior_i != i) {
            leaderboard_t temp = jogadores[i];
            jogadores[i] = jogadores[maior_i];
            jogadores[maior_i] = temp;
        }
    }
}

int atualizar_leaderboard(char *nome_arquivo, int pontuacao, int num_jogadores_leaderboard){//atualiza leaderboard apos gameover

    leaderboard_t jogador[5];
    char novo_nome[20] = {'\0'};

    int i;

    int key; //tecla pressionada
    int continuar_escrita = 1; //1 para permanecer
    int num_letras=0; //num de letras digitadas

    Rectangle caixa_texto = { JANELA_X/2 - 100, JANELA_Y/2, 500, 50 };

    FILE *fp = fopen(nome_arquivo, "rb+");
    if(fp == NULL) return -1;

    fread(jogador, sizeof(leaderboard_t), 5, fp);

    selection_sort(jogador, num_jogadores_leaderboard);

    if(pontuacao > jogador[4].pontos){
        while(continuar_escrita == 1){

            BeginDrawing();

                ClearBackground(BLACK);
                DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

                DrawText("GAMEOVER!", JANELA_X / 2 - (MeasureText("GAMEOVER!", 30)/2), 20, 30, RED);

                DrawText("Leaderboard prévio", 100, 70, 30, WHITE);

                for (i = 0; i < num_jogadores_leaderboard; i++) {
                    char str_temp[100]; //string que aparecera na tela
                    snprintf(str_temp, 100, "%d. %s - %d", i + 1, jogador[i].nome, jogador[i].pontos);
                    Color cor;
                    switch (i){
                    case 0: cor = GOLD; break;
                    case 1: cor = LIGHTGRAY; break;
                    case 2: cor = BROWN; break;
                    default: cor = DARKGRAY; }
                    DrawText(str_temp, 100+MeasureText("Leaderboard prévio", 30)/2 - MeasureText(str_temp, 20)/2, JANELA_Y/3 + i * 40, 20, cor);
                }

                DrawFPS(10,10);
                DrawText("Nova top 5 pontuação! Insira um nome:", (int)caixa_texto.x + 5, (int)caixa_texto.y -45, 30, WHITE);
                DrawRectangleRec(caixa_texto, LIGHTGRAY);
                DrawRectangleLines((int)caixa_texto.x, (int)caixa_texto.y, (int)caixa_texto.width, (int)caixa_texto.height, RED);
                DrawText(novo_nome, (int)caixa_texto.x + 5, (int)caixa_texto.y + 8, 40, MAROON);
                DrawText(TextFormat("Sua pontuação: %d", pontuacao), (int)caixa_texto.x +5, (int)caixa_texto.y + 75, 20, WHITE);

            EndDrawing();
            key = GetCharPressed();
            while (key > 0){
                if ((key >= 65) && (key <= 122) && (num_letras < 19)){
                    novo_nome[num_letras] = (char) key;
                    novo_nome[num_letras + 1] = '\0';
                    num_letras++;
                }      
                key = GetCharPressed();
            }   
            if(IsKeyPressed(KEY_BACKSPACE)){
                num_letras--;
                if(num_letras < 0){ num_letras = 0;}
                novo_nome[num_letras] = '\0';
            }
            if(IsKeyPressed(KEY_ENTER) && num_letras > 0){
                continuar_escrita = 0;
            }

        }
        strcpy(jogador[4].nome, novo_nome);
        jogador[4].pontos = pontuacao;
        rewind(fp);
        fwrite(jogador, sizeof(leaderboard_t), 5, fp);
    }

    fclose(fp);
    return 0;
}

int tela_gameover(int estado, int pontuacao){

    int i;
    SetTargetFPS(FPS);

    leaderboard_t jogador[5];

    Rectangle botaoRetornar = {JANELA_X/2 - BOTAO_LARG/2, JANELA_Y-100, BOTAO_LARG, BOTAO_ALT};
    Rectangle botaoTeclaM = {JANELA_X/2 - BOTAO_LARG/2 -70, JANELA_Y-100, BOTAO_ALT, BOTAO_ALT};

    unsigned int num_jogadores_leaderboard = carregar_leaderboard("leaderboard.bin", jogador); 
    selection_sort(jogador, num_jogadores_leaderboard);   

    while(estado == GAMEOVER && !WindowShouldClose()){

        Vector2 mousePos = GetMousePosition();

        BeginDrawing();

            ClearBackground(BLACK);
            DrawFPS(10,10);
            DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

            DrawText("GAMEOVER!", JANELA_X / 2 - (MeasureText("GAMEOVER!", 30)/2), 20, 30, RED);

            DrawText("Leaderboard", JANELA_X/2 - MeasureText("Leaderboard", 30)/2, 70, 30, WHITE);

            DrawRectangleRec(botaoRetornar, LIGHTGRAY);
            DrawText("Retornar", JANELA_X / 2  - (MeasureText("Retornar", 20)/2), botaoRetornar.y + 15, 20, BLACK);
            DrawRectangleRec(botaoTeclaM, GRAY);
            DrawText("M", botaoTeclaM.x + 8, botaoTeclaM.y + 2, 50, BLACK);

        if(num_jogadores_leaderboard == -1){ DrawText("Erro ao carregar Leaderboard!",
                        JANELA_X / 2 - (MeasureText("Erro ao carregar Leaderboard!", 26)/2),JANELA_Y/2, 26, RED);}
        else{
            for (i = 0; i < num_jogadores_leaderboard; i++) {
                char str_temp[100]; //string que aparecera na tela
                snprintf(str_temp, 100, "%d. %s - %d", i + 1, jogador[i].nome, jogador[i].pontos);
                Color cor;
                switch (i){
                case 0: cor = GOLD; break;
                case 1: cor = LIGHTGRAY; break;
                case 2: cor = BROWN; break;
                default: cor = DARKGRAY; }
                DrawText(str_temp, JANELA_X/2 - MeasureText(str_temp, 20)/2, JANELA_Y/3 + i * 40, 20, cor);
            }
        }

            DrawText(TextFormat("Sua pontuação: %d", pontuacao), JANELA_X/12, JANELA_Y/2, 20, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, botaoRetornar)) {
                estado = MENU;
            }
        }

        if(pontuacao > jogador[4].pontos){
            estado = MENU; 
        }

        if(IsKeyPressed(KEY_M)) {estado = MENU;}

        EndDrawing();
    }

    atualizar_leaderboard("leaderboard.bin", pontuacao, num_jogadores_leaderboard);

    return estado;
}