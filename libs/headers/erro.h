#ifndef ERRO_H
#define ERRO_H

#include "pokemon.h"
#include "player.h"

/* ======================================
   FUNÇÕES DE EXCEÇÃO (ORIGINAIS)
   ====================================== */
void nome_pokemon_exception(char *nome);
void arquivo_nao_encontrado_exception(char *filename);
void string_nao_encontrada_exception(char *nome, char *filename);
void move_nao_encontrado_exception(char *move, char *filename);
void tipos_exception(char *nome);

/* ======================================
   VALIDAÇÕES DE ENTRADA
   ====================================== */
int validar_tamanho_time(int tamanho);
int validar_nome_player(char *nome);
int validar_nome_pokemon(char *nome);

/* ======================================
   VALIDAÇÕES DE STATS
   ====================================== */
int validar_evs(int *evs);
int validar_ivs(int *ivs);

/* ======================================
   VALIDAÇÕES DE INICIALIZAÇÃO
   ====================================== */
int validar_pokemon_inicializado(Pokemon pokemon, char *nome);
int validar_time_inicializado(Pokemon *time, int tamanho, int player_num);
int validar_players_inicializados(Player player1, Player player2);
int validar_arquivos_necessarios(void);

/* ======================================
   VALIDAÇÕES DE BATALHA
   ====================================== */
int validar_pokemon_em_batalha(Pokemon pokemon, int player_num, int indice);
int validar_move_valido(Move move, char *nome_pokemon);

/* ======================================
   FUNÇÕES DE ALOCAÇÃO COM VALIDAÇÃO
   ====================================== */
void validar_malloc(void *ptr, char *contexto);

#endif