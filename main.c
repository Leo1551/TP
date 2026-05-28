#include <stdio.h>
#include "libs/headers/dialogos.h"
#include "libs/headers/player.h"

int main(){


    // 1º inicialização de variaveis

    Player players[2];
    int *tamanho = get_tamanho_times();

    players[0].time     = criar_time(tamanho[0]);
    players[0].tam_time = tamanho[0];
    
    players[1].time     = criar_time(tamanho[1]);
    players[1].tam_time = tamanho[1];
    
    
    return 0;
}

