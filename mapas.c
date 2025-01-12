#include "mapas.h"
#include <stdio.h>
#include <string.h>

/* CONSTANTES */

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
int le_arq_mapa(const char *nome_arq, char mapa[LINHAS_MAPA][COLUNAS_MAPA]) {
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

        // tirar o '\n' // precisa?
        if (linha[strlen(linha) - 1] == '\n') {
            linha[strlen(linha) - 1] = '\0';
        }

        // verifica se ha um item no inicio de cada secao
        if (i == 0 && valida_secoes(linha) == 0) {
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
