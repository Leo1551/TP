#include <stdio.h>
#include "libs/headers/dialogos.h"
#include "libs/headers/player.h"
#include "libs/headers/erro.h"

int main(){


    Player *players = malloc(sizeof(Player) * 2);
    system("clear");
    int *tamanho = get_tamanho_times();
    
    // Validar tamanho de cada time
    if (!validar_tamanho_time(tamanho[0]) || !validar_tamanho_time(tamanho[1])) {
        return 1;
    }
    
    //system("clear");
    // Inicializar jogador 1
    printf("\n\n+=================(Jogador 1)=====================+\n\n");
    players[0].nome    = inserir_nome();
    players[0].time     = criar_time(tamanho[0]);
    players[0].tam_time = tamanho[0];

    //system("clear");

    printf("\n\n+=================(Jogador 2)=====================+\n\n");
    //Inicializar jogador 2
    players[1].nome     = inserir_nome();
    players[1].time     = criar_time(tamanho[1]);
    players[1].tam_time = tamanho[1];
    
    for (int i = 0; i < players[0].tam_time; i++)
        show_info(players[0].time[i]);

    system("sleep 9");
    system("clear");
    for (int i = 0; i < players[1].tam_time; i++)
        show_info(players[1].time[i]);
    system("sleep 10");
    system("clear");

    // Validar que ambos os players foram inicializados corretamente
    if (!validar_players_inicializados(players[0], players[1])) {
        printf("\n\n Falha ao inicializar um ou ambos os jogadores\n");
        return 1;
    }
    
    iniciar_batalha(&players[0], &players[1]);
    
    free(tamanho);
    return 0;
}

