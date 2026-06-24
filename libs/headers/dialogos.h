#ifndef DIALOGOS_H
#define DIALOGOS_H

#include "pokemon.h"
#include "player.h"

// Para inicializar players
char* inserir_nome();           //  nome
int* get_tamanho_times(void);       //  tam_time
Pokemon* criar_time(int tamanho);   //  time[tam_time]


//possiveis patches futuros
void aviso_de_preguica_do_dev();


// para a batalha em si
int* captar_stats(int max_por_stat, int max_total);
void iniciar_batalha(Player *player1, Player *player2);
void gerar_log_pos_batalha(Player *player1, Player *player2, char *log);


//declarações esquecidas
char *gerar_log_pre_batalha(Player *player1, Player *player2);
int quem_vence(Pokemon *poke1, Pokemon *poke2, int *turno, char *log, char *arq);
void gerar_log_vencedor_batalha(Player *player1, char *arq, int num); // see Pattern 3 conflict below
void gerarTurno(Pokemon *poke1, Pokemon *poke2, int *turno, char *log);
void gerar_log_turno(char *acoes, char *arq);
int captar_indice_move(Pokemon *poke);
void calcular_turno(Pokemon *poke1, int move1, Pokemon *poke2, int move2, char *log);
char *gerar_condicoes_pos_turno(Pokemon *poke, Pokemon *inimigo);
void apply_burn(Pokemon *poke);
void apply_poison(Pokemon *poke);
int apply_badly_poison(Pokemon *poke);
void cause_leechseed(Pokemon *inimigo, Pokemon *poke);

void erro_ao_inicializar_pokemon(char *nome_buf);

#endif 
