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
#define LINHAS_SECAO   12
#define COLUNAS_SECAO  30

/* FUNCOES (implementadas em mapas.c) */

// somente as que necessitam ser usadas fora de mapas.c...

int char_representa_item(char c);
