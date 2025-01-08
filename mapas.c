//#include "raylib.h" // temp
#include "mapas.h"
#include <stdio.h>
#include <string.h>

/* CONSTANTES */

// Apenas para desenvolvimento
#define SALVAR_CARACTERES_INVALIDOS_COMO_ESPACO 0

// Auxiliar
#define TAM_LINHA COLUNAS_MAPA + 2 // '\n' e '\0'

/* FUNCOES */

// Retorna um "booleano" dizendo se o caractere representa um item (espaco vazio nao eh considerado item)
int char_representa_item(char c) {
    return c == CHAR_PAREDE || c == CHAR_MOEDA || c == CHAR_ESPINHO; // caracteres diferentes serao considerados espaco vazio
}

// Retorna 1 se o caractere eh valido no mapa
int char_valido_mapa(char c) {
    return c == CHAR_ESPACO || char_representa_item(c);
}

// Verifica se cada secao possui um item na sua primeira posicao (necessario para a geracao do jogo)
int valida_secoes(char primeira_linha[]) {
    int j;

    for (j = 0; j < COLUNAS_MAPA; j += COLUNAS_SECAO) {
        if (!char_representa_item(primeira_linha[j])) {
            return 0;
        }
    }

    return 1;
}

// Le o mapa e salva em uma matriz, com todas as validacoes
// Retorna 1 se deu tudo certo e 0 caso contrario
int le_arq_mapa(char nome_arq[], char mapa[LINHAS_MAPA][COLUNAS_MAPA]) {
    FILE *arq = fopen(nome_arq, "r");
    int i, j;
    char linha[TAM_LINHA] = {0};

    if (arq == NULL) {
        return 0;
    }

    // pegar o conteudo do mapa, linha por linha
    for (i = 0; i < LINHAS_MAPA; i++) {
        char *p = fgets(linha, TAM_LINHA, arq);

        if (p == NULL) {
            return 0;
        }

        // tirar o '\n'
        if (linha[strlen(linha) - 1] == '\n') {
            linha[strlen(linha) - 1] = '\0';
        }

        // verifica se ha um item no inicio de cada secao
        if (i == 0 && valida_secoes(linha) == 0) { // combinar no outro if? pode ficar menos legivel...
            return 0;
        }

        // ver e salvar os caracteres da linha
        for (j = 0; j < COLUNAS_MAPA; j++) {
            char c = linha[j];

            if (char_valido_mapa(c)) {
                mapa[i][j] = c;
            } else {
                mapa[i][j] = CHAR_ESPACO; // se nao eh char valido, considerado espaco vazio
            }
        }
    }

    fclose(arq);

    return 1;
}

/* MAIN (para testes) */
/*int main() { // temp
    InitWindow(1600, 400, "Arquivos de mapa");
    SetTargetFPS(60);

    char nome_arq[] = "resources/mapas/teste.txt";
    char mapa[LINHAS_MAPA][COLUNAS_MAPA] = {0};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Leu mapa \"%s\": %d", nome_arq, le_arq_mapa(nome_arq, mapa)), 0, 0, 40, BLACK);

        // desenha o mapa
        for (int i = 0; i < LINHAS_MAPA; i++) {
            for (int j = 0; j < COLUNAS_MAPA; j++) {
                int font_size = 6;
                char c = mapa[i][j];
                DrawText(TextFormat("%c", c == ' ' ? '_' : c), j * font_size * 1.1, 60 + i * font_size, font_size, BLACK);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}*/
