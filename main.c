#include "raylib.h"
#include "sadride.h"

typedef struct{
    int x, y;
    float velocidade;
    int largura, altura;
    Texture2D textura; //adicionar png
}personagem_t;

personagem_t inicializar_personagem() {
    personagem_t jetpack;
    jetpack.x = JANELA_X/6;
    jetpack.y = JANELA_Y;
    jetpack.velocidade = 0;
    //interessante deixar tamanho do jetpack como variavel caso tenhamos um powerup
    jetpack.largura = 40;
    jetpack.altura = 40;
    jetpack.textura = LoadTexture("resources/sprite_coracao.png");
    return jetpack;
}

void textoCentralizado (char *texto, int fonteTamanho, int posY, Color cor) {
    int larguraTexto = MeasureText(texto, fonteTamanho);
    DrawText(texto, JANELA_X / 2 - larguraTexto / 2, posY, fonteTamanho, cor);
}

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

void inicializar_jogo () {
    InitWindow(JANELA_X, JANELA_Y, "Jetpack Sadride");
    SetExitKey(0); // O ESC nao fecha mais a janela
    DisableCursor(); HideCursor();
    SetTargetFPS(60); // fps do jogo
}

EstadoJogo loop_jogo (personagem_t *jetpack, bool *isPaused) {
    EstadoJogo estado = JOGO;
    *isPaused = false;

    while(estado == JOGO && !WindowShouldClose()) {
        if(IsKeyPressed(KEY_ESCAPE)) {  // mudar facilmente o estado de pausa
            *isPaused = !*isPaused;
        }

        DrawFPS(10,10);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!*isPaused) { // quando está rodando
            movimento(&jetpack->y, &jetpack->velocidade, jetpack->altura);
            //DrawRectangle(jetpack.x, jetpack.y, jetpack.largura, jetpack.altura, BLACK);
        }

        //Tudo isso pra ajeitar a dimensao de um png de 400x400, vou ver se só da pra ser assim
        //provavermente fazer uma função pra carregar texturas mais tarde
        Rectangle source = {0, 0, jetpack->textura.width, jetpack->textura.height};
        Rectangle dest = {jetpack->x, jetpack->y, jetpack->largura, jetpack->altura};
        Vector2 origin = {0, 0};
        DrawTexturePro(jetpack->textura, source, dest, origin, 0.0f, WHITE);

        // jogo pausado
        if (*isPaused) {  // Measure Text pra ficar centralizado bem bunitinhu
            textoCentralizado("Jogo Pausado", 30, JANELA_Y/2 - 20, WHITE);
            textoCentralizado("Pressione ESC para continuar ou M para retornar ao menu",
                20, JANELA_Y/2 + 20, GRAY);
            if(IsKeyPressed(KEY_M)) {estado = MENU;}
        }

        EndDrawing();
    }
    return estado;
}

int main(){

    inicializar_jogo ();
    EstadoJogo estado = MENU;
    personagem_t jetpack;
    bool isPaused = false;      //variavel de controle para pausa

    while(estado != SAIR){
        if(estado == MENU) {
            estado = tela_inicial();
        }

        if(estado == JOGO) {  // verificar se o jogo deve ser rodado
            jetpack = inicializar_personagem();
            estado = loop_jogo(&jetpack, &isPaused);
        }

        if (WindowShouldClose()) { // para poder fechar o jogo apertando o botao fechar janela
            estado = SAIR;  //só sair
            break; // sair do loop, nao sei se precisa, mas vai que ne
        }
    

        UnloadTexture(jetpack.textura);
    }
    CloseWindow();
    return 0;
}