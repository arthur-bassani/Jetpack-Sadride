/*
>> Codigo para criar um arquivo binário com o leaderboard <<
- A ordem decrescente nao esta sendo feita no arquivo, mas sim quando
a ordem eh exposta na tela de leaderboard no menu.
- (talvez?) Isso permite que facamos uma troca mais simples qnd
houver a nessecidade em Novo Jogo
*/

#include <stdio.h>
#include "sadride.h"

int main() {
    FILE *fp = fopen("leaderboard.bin", "wb");

    leaderboard_t jogador[5] = {
        {"Lucas", 123456789},
        {"Arthur", 2005},
        {"Thiago", 999999999},
        {"Eduardo", 2006},
        {"Pedro", 2004}
    };

    fwrite(jogador, sizeof(leaderboard_t), 5, fp);
    fclose(fp);

    return 0;
}