/* CONSTANTES */

// Caracteres do mapa
#define CHAR_PAREDE   'X'
#define CHAR_MOEDA    'C'
#define CHAR_ESPINHO  'Z'
#define CHAR_ESPACO   ' '

// Matriz mapa
#define LINHAS_MAPA    12
#define COLUNAS_MAPA   240

// Matriz de secao
#define LINHAS_SECAO   LINHAS_MAPA
#define COLUNAS_SECAO  30

/* FUNCOES (implementadas em mapas.c) */

int char_representa_item(char c);

int le_arq_mapa(const char *nome_arq, char mapa[LINHAS_MAPA][COLUNAS_MAPA]);
