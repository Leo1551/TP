#include <stdio.h>
#include "libs/headers/dialogos.h"
#include "libs/headers/player.h"
#include "libs/headers/erro.h"

int main(){


    Player players[2];
    int *tamanho = get_tamanho_times();
    
    // Validar tamanho de cada time
    if (!validar_tamanho_time(tamanho[0]) || !validar_tamanho_time(tamanho[1])) {
        return 1;
    }
    
    // Inicializar jogador 1
    players[0].nome     = inserir_nome();
    players[0].time     = criar_time(tamanho[0]);
    players[0].tam_time = tamanho[0];
    
    //Inicializar jogador 2
    players[1].nome     = inserir_nome();
    players[1].time     = criar_time(tamanho[1]);
    players[1].tam_time = tamanho[1];
    
    // Validar que ambos os players foram inicializados corretamente
    if (!validar_players_inicializados(players[0], players[1])) {
        printf("\n\n Falha ao inicializar um ou ambos os jogadores\n");
        return 1;
    }
    
    iniciar_batalha(&players[0], &players[1]);
    
    free(tamanho);
    return 0;
}

