#include "sadride.h"
#include <stdlib.h>
#include <time.h>

typedef struct{
    int x, y;
    float velocidade;
    int largura, altura;
    Texture2D textura; //adicionar png
}personagem_t;

typedef struct {
    Rectangle hitbox;
    float velocidade;
} missil_t;

/*typedef enum {
    INATIVO,
    ARMADO,
    DISPARADO
} EstadoLaser;

typedef struct {
    Rectangle hitbox;
    EstadoLaser estado;
} laser_t;*/

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
    InitAudioDevice();
    SetExitKey(0); // O ESC nao fecha mais a janela
    DisableCursor(); HideCursor();
    SetTargetFPS(FPS); // fps do jogo
}

// Retorna o personagem como um retangulo para ver as colisoes
Rectangle personagem_retangulo(personagem_t *jetpack) {
    return (Rectangle) {jetpack->x, jetpack->y, jetpack->largura, jetpack->altura};
}

// Checa as colisoes com um item do fundo
void checar_colisoes_item(personagem_t *jetpack, item_t *item, int *moedas_coletadas, Sound som_moeda) {
    Rectangle hitbox_jp = personagem_retangulo(jetpack);

    if (item->tipo == CHAR_MOEDA) {
        circulo_t hitbox_item = item_circulo(item);
        
        if (CheckCollisionCircleRec(hitbox_item.centro, hitbox_item.raio, hitbox_jp)) {
            (*moedas_coletadas)++;
            item->tipo = CHAR_ESPACO;
            PlaySound(som_moeda);
        }
    } else if (char_representa_item(item->tipo)) {
        Rectangle hitbox_item = item_retangulo(item);

        if (CheckCollisionRecs(hitbox_item, hitbox_jp)) {
            switch (item->tipo) {
            case CHAR_PAREDE:
                if (jetpack->y  <= hitbox_item.y) {
                    //colisao vinda de baixo (personagem bate no teto do item)
                    jetpack->y = hitbox_item.y - jetpack->altura;
                    jetpack->velocidade = 0;
                } else if (jetpack->y >=  hitbox_item.y) {
                    //colisao vinda de cima (personagem bate no chao do item)
                    jetpack->y = hitbox_item.y + hitbox_item.height;
                    jetpack->velocidade = 0;
                }
                break;
            case CHAR_ESPINHO:
                
                //PlaySound(som_dano);
                break;
            }
        }
    }
}

// Pega o pelo arquivo, usando a fase informada
int pegar_mapa(int fase, char mapa[LINHAS_MAPA][COLUNAS_MAPA]) {
    return le_arq_mapa(TextFormat("resources/mapas/mapa%d.txt", fase), mapa);
}

// Retorna 1 se consegue pegar o mapa pelo arquivo, usando uma matriz de caracteres auxiliar
int consegue_pegar_mapa(int fase) {
    char mapa[LINHAS_MAPA][COLUNAS_MAPA] = {0};
    return pegar_mapa(fase, mapa);
}

// Desenha os textos da pontuacao
void escrever_pontuacao(int pontuacao) {
    DrawText(TextFormat("%08d", pontuacao), TXT_PONT_X, TXT_PONT_Y, TXT_PONT_FONTE, TXT_PONT_COR);
    //DrawText(TextFormat("Distância percorrida: %08d", distancia_percorrida), TXT_PONT_X, TXT_DIST_Y, TXT_PONT_FONTE, TXT_PONT_COR);
    //DrawText(TextFormat("Moedas coletadas: %08d", moedas_coletadas), TXT_PONT_X, TXT_MOEDAS_Y, TXT_PONT_FONTE, TXT_PONT_COR);
}

// MISSIL
int deve_lancar_missil(int distancia_percorrida, int fase) {
    return distancia_percorrida / (4 - fase) % FATOR_FREQ_MISSIL == 0;
}

void lancar_missil(missil_t *missil, int y_min, int y_max, float velocidade) {
    missil->hitbox.x = JANELA_X;
    missil->hitbox.y = y_min + rand() % (y_max - y_min + 1);
    missil->velocidade = velocidade;
}

int ha_missil_tela(missil_t *missil) {
    return missil->hitbox.x + missil->hitbox.width >= 0;
}

void mover_missil(missil_t *missil) {
    missil->hitbox.x -= (int) missil->velocidade;
}

// LASER
/*
int posicao_laser(int linha_max, int linha_min, int espessura) {
    return linha_min + rand() % (linha_max - linha_min + 1);
}

double armar_laser(Rectangle *laser) {
    return GetTime();
}*/

void disparar_laser() {}

EstadoJogo loop_jogo (personagem_t *jetpack, bool *isPaused) {
    EstadoJogo estado = JOGO;
    *isPaused = false;

    // sons
    Sound som_moeda = LoadSound("resources/sons/moeda.wav");
    Sound som_fase = LoadSound("resources/sons/fase.wav");
    //Sound som_dano = LoadSound("resources/sons/dano.wav");
    Sound som_missil = LoadSound("resources/sons/missil.wav");
    //Music musica = LoadMusicStream("resources/sons/musica.wav"); // ?

    // fundo
    char mapa[LINHAS_MAPA][COLUNAS_MAPA] = {0};
    int i, andou_um;
    float velocidade_mapa = VEL_INICIAL_MAPA;
    int distancia_percorrida = 0, moedas_coletadas = 0, pontuacao = 0;
    int atual_x_antes, proxima_x_antes;
    int fase = 1; // identifica o mapa sendo usado

    // secoes
    item_t atual[MAX_ITENS] = {0};
    item_t proxima[MAX_ITENS] = {0};

    // eventos
    missil_t missil = {{-2 * JANELA_X, 0, TAM_TILE, TAM_TILE}}; // inicializa o missil fora da tela

    srand(RAND_SEED);

    // atribuir mapa
    if (pegar_mapa(1, mapa) != 1) {
        // erro! voltar para o menu...
        CloseWindow(); // temp
    }

    // gerar secoes iniciais
    gerar_secao(atual, mapa, 0, 0);
    gerar_secao(proxima, mapa, inicio_secao_aleatorio(), COLUNAS_SECAO * TAM_TILE);

    while(estado == JOGO && !WindowShouldClose()) {
        if(IsKeyPressed(KEY_ESCAPE)) {  // mudar facilmente o estado de pausa
            *isPaused = !*isPaused;
        }

        DrawFPS(10,10); // coisas sao desenhadas em cima...
        BeginDrawing();
        ClearBackground(BLACK);

        // Fundo
        andou_um = 0;

        for (i = 0; i < MAX_ITENS; i++) { // MAX_ITENS pode ser ineficiente... mas deixa assim
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

            // colisoes
            checar_colisoes_item(jetpack, &atual[i], &moedas_coletadas, som_moeda);
            checar_colisoes_item(jetpack, &proxima[i], &moedas_coletadas, som_moeda);
        }

        if (VER_TILES) { // dev, para ver as secoes
            DrawRectangleLines(atual[0].x, atual[0].y, COLUNAS_SECAO * TAM_TILE, LINHAS_SECAO * TAM_TILE, RED);
        }

        // Escreve a pontuacao na tela
        // eh interessante que sejam a ultima coisa a ser desenhada, para ficar bem na frente
        escrever_pontuacao(pontuacao);

        if (VELOCIMETRO) {
            const char *text = TextFormat("%09.6f", velocidade_mapa / TAM_TILE);
            DrawText(text, COLUNAS_SECAO * TAM_TILE - MeasureText(text, TXT_PONT_FONTE) - TXT_PONT_X, TXT_PONT_Y, TXT_PONT_FONTE, WHITE);
        }
        
        // Contar distancia de um tile andada
        if (andou_um) {
            distancia_percorrida++;

            // Atualiza velocidade
            if (VEL_MAPA_VARIAVEL) {
                aumentar_velocidade(&velocidade_mapa, PASSO_VEL_MAPA, VEL_MAX_MAPA);
            }

            // Eventos
            // Missil
            if (distancia_percorrida >= DIST_MIN_MISSIL && !ha_missil_tela(&missil) && deve_lancar_missil(distancia_percorrida, fase)) {
                lancar_missil(&missil, 2 * TAM_TILE, 9 * TAM_TILE, velocidade_mapa * FATOR_VEL_MISSIL);
                PlaySound(som_missil);
            }

            // Laser
            /*if (fase >= 2) {
                ;
            }*/
        }

        if (ha_missil_tela(&missil)) {
            if (!*isPaused) {
                mover_missil(&missil);
            }
            
            if (ha_missil_tela(&missil) && CheckCollisionRecs(personagem_retangulo(jetpack), missil.hitbox)) {
                // game over
            }
            
            DrawRectangleRec(missil.hitbox, COR_MISSIL);
        }

        // Calculo da pontuacao
        // no pdf eh a distancia que eh multiplicada por 10, mas acho que assim faz mais sentido...
        pontuacao = distancia_percorrida + 10 * moedas_coletadas;

        // Fazer o "deslizamento" se a secao atual sai da tela
        if (proxima[0].x <= 0) {
            // passar de fase (trocar de mapa)
            int mudou_fase = 0;
            if (fase == 1 && pontuacao >= PONT_FASE_2 && consegue_pegar_mapa(2)) {
                fase = 2;
                mudou_fase = 1;
            } else if (fase == 2 && pontuacao >= PONT_FASE_3 && consegue_pegar_mapa(3)) {
                fase = 3;
                mudou_fase = 1;
            }

            if (mudou_fase) {
                PlaySound(som_fase);
                pegar_mapa(fase, mapa);
            }

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
                20, JANELA_Y/2 + 20, WHITE);
            if(IsKeyPressed(KEY_M)) {estado = MENU;}
        }

        EndDrawing();
    }

    // descarregar sons
    UnloadSound(som_moeda);
    UnloadSound(som_fase);
    UnloadSound(som_missil);

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
