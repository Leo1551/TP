#ifndef DIALOGOS_H
#define DIALOGOS_H

#include "pokemon.h"
#include "player.h"

int* get_tamanho_times(void);
Pokemon* criar_time(int tamanho);
int* captar_stats(int max_por_stat, int max_total);
void iniciar_batalha(Player player1, Player player2);

#endif 
