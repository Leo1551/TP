#ifndef DIALOGOS_H
#define DIALOGOS_H

#include "pokemon.h"
#include "player.h"

// Para inicializar players
char* inserir_nome(void);           //  nome
int* get_tamanho_times(void);       //  tam_time
Pokemon* criar_time(int tamanho);   //  time[tam_time]


//possiveis patches futuros
void aviso_de_preguica_do_dev();


// para a batalha em si
int* captar_stats(int max_por_stat, int max_total);
void iniciar_batalha(Player player1, Player player2);
int quem_vence(Pokemon poke1, Pokemon poke2, char *log);
void gerarTurno(Pokemon *poke1, Pokemon *poke2, int *turno, char *log);
void gerar_log_pos_batalha(Player player1, Player player2);


#endif 
