/* A FAZER:
 * Usar os mapas por arquivo
 * Determinar pontuacao para passar de fase
 */

#include "raylib.h"
#include "sadride.h"
#include <stdlib.h>

/* FUNCOES */

// Gera e retorna um valor aleatorio para ser usado como inicio da secao
int inicio_secao_aleatorio() {
    // assumindo que o mapa possui o tamanho certo...
    // fazer algo mais sofisticado, numeros do intervalo que nao podem ser gerados...
    return rand() % (COLUNAS_MAPA / COLUNAS_SECAO) * COLUNAS_SECAO; // 0, 30, 60, 90, 120, 150, 180, 210.
}

// Gera completamente uma secao, com o conteudo e os itens
// Necessariamente cada secao deve possuir um elemento no (0, 0) da matriz, para que o programa funcione
// retorno?
void gerar_secao(item_t itens[MAX_ITENS], char mapa[LINHAS_MAPA][COLUNAS_MAPA], int j_inicial_mapa, int j_offset_tela) {
    // assumindo que j_inicial_mapa eh valido...
    
    int i, j;
    int item = 0; // salvar a quantidade de itens se necessario...

    for (i = 0; i < LINHAS_SECAO; i++) {
        for (j = 0; j < COLUNAS_SECAO; j++) {
            char c = mapa[i][j_inicial_mapa + j];

            if (char_representa_item(c)) {
                itens[item].tipo = c;
                itens[item].x = j * TAM_TILE + j_offset_tela;
                itens[item].y = i * TAM_TILE;
                
                item++;
            }
        }
    }
}

// Copia os itens de um vetor para o outro
// retorno?
void copiar_itens(item_t destino[], item_t origem[]) {
    int i;

    // limpar destino?

    for (i = 0; i < MAX_ITENS; i++) {
        destino[i] = origem[i];
    }
}

// Retorna a cor que o elemento deve ter na tela (depois pode ser com imagem...)
Color cor_tile(char tile) {
    Color cor;

    switch(tile) {
    case CHAR_PAREDE:
        cor = BLACK;
        break;
    case CHAR_MOEDA:
        cor = GOLD;
        break;
    case CHAR_ESPINHO:
        cor = BROWN;
        break;
    case CHAR_ESPACO: // nao eh usado
        cor = LIGHTGRAY;
        break;
    default:
        cor = MAGENTA; // so para indicar que algo deu errado
        break;
    }

    return cor;
}

// Desenha um item, se ele estiver na tela
// retorno?
void desenhar_item(item_t *item) {
    int largura = TAM_TILE, altura = TAM_TILE; // caso precise mudar... (provavelmente nao vai, mas fica mais legivel)
    
    if (char_representa_item(item->tipo) && item->x + largura > 0) {
        DrawRectangle(item->x, item->y, largura, altura, cor_tile(item->tipo));

        if (VER_TILES) { // dev, para ver o contorno dos tiles dos itens
            DrawRectangleLines(item->x, item->y, largura, altura, GRAY);
        }
    }
}

// Retorna um "booleano" que indica se o item passou pelo inicio da tela
int item_passou_inicio_tela(int x_antes, int x_agora) {
    return x_antes > 0 && x_agora <= 0;
}

// Desenha os textos da pontuacao
// mostrar soh a pontuacao? (eh o que eh exigido...)
// talvez soh o numero...
// quantidade de zeros depende da pontuacao para passar de fase...
void escrever_pontuacao(int pontuacao, int distancia_percorrida, int moedas_coletadas) {
    DrawText(TextFormat("Pontuação: %08d", pontuacao), TXT_PONT_X, TXT_PONT_Y, TXT_PONT_FONTE, TXT_PONT_COR);
    DrawText(TextFormat("Distância percorrida: %08d", distancia_percorrida), TXT_PONT_X, TXT_DIST_Y, TXT_PONT_FONTE, TXT_PONT_COR);
    DrawText(TextFormat("Moedas coletadas: %08d", moedas_coletadas), TXT_PONT_X, TXT_MOEDAS_Y, TXT_PONT_FONTE, TXT_PONT_COR);
}

// Aumenta a velocidade do mapa
// Retorna 0 se a velocidade ja eh maxima e nao foi alterada e 1 no caso contrario
int aumentar_velocidade(float *velocidade_mapa, float acrescimo, float maximo) {
    if (*velocidade_mapa == maximo) {
        return 0;
    }

    if (*velocidade_mapa + acrescimo <= maximo) {
        *velocidade_mapa += acrescimo;
    } else {
        *velocidade_mapa = maximo;
    }

    return 1;
}

// Passa os itens da proxima secao para a atual e gera uma nova proxima
void deslizamento_secoes(item_t atual[], item_t proxima[], char mapa[LINHAS_MAPA][COLUNAS_MAPA]) {
    copiar_itens(atual, proxima);
    gerar_secao(proxima, mapa, inicio_secao_aleatorio(), atual[0].x + COLUNAS_SECAO * TAM_TILE);
}
