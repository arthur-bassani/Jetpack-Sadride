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

    switch (tile) {
    case CHAR_PAREDE:
        cor = WHITE;
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

// Retorna o retangulo que o item eh na tela
Rectangle item_retangulo(item_t *item) {
    return (Rectangle) {item->x, item->y, TAM_TILE, TAM_TILE};
}

// Retorna um circulo inscrito no retangulo que item_retangulo retornaria
circulo_t item_circulo(item_t *item) {
    int raio = TAM_TILE / 2;
    return (circulo_t) {(Vector2) {item->x + raio, item->y + raio}, PC_DIAM_MOEDA * raio};
}

// Desenha um item, se ele estiver na tela
void desenhar_item(item_t *item) {
    Rectangle ret = item_retangulo(item);

    if (char_representa_item(item->tipo) && item->x + ret.width > 0) {
        if (item->tipo == CHAR_MOEDA) {
            circulo_t circulo = item_circulo(item);
            DrawCircle(circulo.centro.x, circulo.centro.y, circulo.raio, cor_tile(item->tipo));
        } else {
            DrawRectangleRec(item_retangulo(item), cor_tile(item->tipo));
        }

        if (VER_TILES) { // dev, para ver o contorno dos tiles dos itens
            DrawRectangleLines(ret.x, ret.y, ret.width, ret.height, GRAY);
        }
    }
}

// Retorna um "booleano" que indica se o item passou pelo inicio da tela
int item_passou_inicio_tela(int x_antes, int x_agora) {
    return x_antes > 0 && x_agora <= 0;
}

// Aumenta a velocidade do mapa
void aumentar_velocidade(float *velocidade_mapa, float acrescimo, float maximo) {
    if (*velocidade_mapa + acrescimo <= maximo) {
        *velocidade_mapa += acrescimo;
    } else {
        *velocidade_mapa = maximo;
    }
}

// Passa os itens da proxima secao para a atual e gera uma nova proxima
void deslizamento_secoes(item_t atual[], item_t proxima[], char mapa[LINHAS_MAPA][COLUNAS_MAPA]) {
    copiar_itens(atual, proxima);
    gerar_secao(proxima, mapa, inicio_secao_aleatorio(), atual[0].x + COLUNAS_SECAO * TAM_TILE);
}
