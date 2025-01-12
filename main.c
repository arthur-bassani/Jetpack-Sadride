#include "raylib.h"
#include "sadride.h"
#include <stdlib.h>
#include <time.h>

typedef struct{
    int x, y;
    float velocidade;
    int largura, altura;
    Texture2D textura; //adicionar png
}personagem_t;

personagem_t inicializar_personagem() {
    personagem_t jetpack;
    jetpack.x = JANELA_X/6;
    jetpack.y = JANELA_Y/2;
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
    SetTargetFPS(FPS); // fps do jogo
}

Rectangle personagem_retangulo(personagem_t *jetpack) {
    return (Rectangle) {jetpack->x, jetpack->y, jetpack->largura, jetpack->altura};
}

// Checa as colisoes com os itens do fundo
void checar_colisoes_itens(personagem_t *jetpack, item_t itens[], int *moedas_coletadas) {
    int i;
    Rectangle hitbox_jp = personagem_retangulo(jetpack);

    for (i = 0; i < MAX_ITENS; i++) {
        if (itens[i].tipo == CHAR_MOEDA) {
            circulo_t hitbox_item = item_circulo(&itens[i]);
            
            if (CheckCollisionCircleRec(hitbox_item.centro, hitbox_item.raio, hitbox_jp)) {
                (*moedas_coletadas)++;
                itens[i].tipo = CHAR_ESPACO;
            }
        } else if (char_representa_item(itens[i].tipo)) {
            Rectangle hitbox_item = item_retangulo(&itens[i]);

            if (CheckCollisionRecs(hitbox_item, hitbox_jp)) {
                switch (itens[i].tipo) {
                case CHAR_PAREDE:
                    // colisao vertical...
                    // se for em cima ou em baixo do jogador...
                    break;
                case CHAR_ESPINHO:
                    // acabar...
                    break;
                }
            }
        }
    }
}

EstadoJogo loop_jogo (personagem_t *jetpack, bool *isPaused) {
    EstadoJogo estado = JOGO;
    *isPaused = false;

    // fundo
    char mapa[LINHAS_MAPA][COLUNAS_MAPA] = {0};

    // secoes
    item_t atual[MAX_ITENS] = {0};
    item_t proxima[MAX_ITENS] = {0};

    int i, andou_um;
    float velocidade_mapa = VEL_INICIAL_MAPA;
    int distancia_percorrida = 0, moedas_coletadas = 0, pontuacao = 0;
    int atual_x_antes, proxima_x_antes;

    int n_mapa = 1; // identifica o mapa sendo usado

    srand(RAND_SEED);

    // atribuir mapa
    if (le_arq_mapa(TextFormat("resources/mapas/mapa%d.txt", n_mapa), mapa) != 1) {
        // erro! voltar para o menu...
    }

    // gerar secoes iniciais
    gerar_secao(atual, mapa, 0, 0); // primeiro 0 faz com que a primeira secao seja a primeira do mapa.
                                    // no mapa1 eh a mais tranquila, faz sentido comecar, 
                                    // e isso pode ser um padrao.
    gerar_secao(proxima, mapa, inicio_secao_aleatorio(), COLUNAS_SECAO * TAM_TILE);

    while(estado == JOGO && !WindowShouldClose()) {
        if(IsKeyPressed(KEY_ESCAPE)) {  // mudar facilmente o estado de pausa
            *isPaused = !*isPaused;
        }

        DrawFPS(10,10);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Fundo
        andou_um = 0;

        for (i = 0; i < MAX_ITENS; i++) {
            // MAX_ITENS pode ser ineficiente... mas deixa assim

            // Desenhar itens
            desenhar_item(&atual[i]);
            desenhar_item(&proxima[i]);

            if (!*isPaused) {
                atual_x_antes = atual[i].x;
                proxima_x_antes = proxima[i].x;

                // Mover itens
                atual[i].x -= (int) velocidade_mapa;
                proxima[i].x -= (int) velocidade_mapa;

                // Ver se "o jogador" andou um tile
                if (item_passou_inicio_tela(atual_x_antes, atual[i].x) || 
                    item_passou_inicio_tela(proxima_x_antes, proxima[i].x)) {
                    andou_um = 1;
                }
            }
        }

        if (VER_TILES) { // dev, para ver as secoes
            DrawRectangleLines(atual[0].x, atual[0].y, COLUNAS_SECAO * TAM_TILE, LINHAS_SECAO * TAM_TILE, RED);
        }

        // Escreve a pontuacao na tela
        // eh interessante que sejam a ultima coisa a ser desenhada, para ficar bem na frente
        escrever_pontuacao(pontuacao, distancia_percorrida, moedas_coletadas);

        if (VELOCIMETRO) {
            const char *text = TextFormat("%09.6f", velocidade_mapa / TAM_TILE);
            DrawText(text, COLUNAS_SECAO * TAM_TILE - MeasureText(text, TXT_PONT_FONTE) - TXT_PONT_X, TXT_PONT_Y, TXT_PONT_FONTE, LIME);
        }
        
        // Contar distancia de um tile andada
        if (andou_um) {
            distancia_percorrida++;

            // Atualiza velocidade
            if (VEL_MAPA_VARIAVEL) {
                aumentar_velocidade(&velocidade_mapa, PASSO_VEL_MAPA, VEL_MAX_MAPA);
            }
        }

        // colisoes
        checar_colisoes_itens(jetpack, atual, &moedas_coletadas);
        checar_colisoes_itens(jetpack, proxima, &moedas_coletadas);

        // Calculo da pontuacao
        // no pdf eh a distancia que eh multiplicada por 10, mas acho que assim faz mais sentido...
        pontuacao = distancia_percorrida + 10 * moedas_coletadas;

        // passar de fase? if (pontuacao >= ...)
        // trocar o mapa...

        // Fazer o "deslizamento" se a secao atual sai da tela
        if (proxima[0].x <= 0) {
            deslizamento_secoes(atual, proxima, mapa);
        }

        // Personagem

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

int main() {

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
