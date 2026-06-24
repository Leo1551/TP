#include "pokemon.h"
#include <stdio.h>
#ifndef UTILIDADES_H
#define UTILIDADES_H

// Utilidades gerais de string e arquivos
void limpar_buffer_de_teclado(void);
void capitalizarPalavras(char *str);
void trocar_quebra_de_linha_por_terminador(char *str);
FILE* achar_string_em_arquivo(const char *nome, char *filename);
int calc_bst(int bst, int ev, int iv);
int calc_hp(int bst, int ev, int iv);

// Conversões e cálculos (definidas em utilidades.c)
char* int_type_to_string(int type);


// para pokemon.c -> show_stats
char* show_status_condition(SCondition condition);
char* int_type_to_string(int type);
char* int_category_to_string(int categoria);
char* other_conditions_to_string(MoveCondition *conditions, int tam);
// para dialogos.c -> iniciar_batalha
int search_indice_move(Move *moves, char *move);

//fonte: https://calc.pokemonshowdown.com/, https://www.reddit.com/r/gamedesign/comments/hvrmyz/pok%C3%A9mon_damage_calculation_analysis/?tl=pt-br
/*
    Direciona moves de categoria STATUS de PHYSICAL/SPECIAL
*/
void move_calc(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log);
/*
    Calcula o dano de moves PHYSICAL/SPECIAL
*/            
void dmg_calc(Pokemon *ataca, Pokemon *recebe, int indice, char *log, int *dano_total);
/*
    Realiza todos os efeitos de moves
    será a última parte a ser desenvolvida.
*/
void effect_calc(Pokemon *ataca, Pokemon *recebe, char *str_effect, char *log);
/*
    Essa função é uma compressão dos seguintes fatores multiplicadores de dano:
        - Roll (calculo da porcentagem de damage output que realmente será aplicado)
        - Chance de critical hit
        - Multiplicadores de ataque e defesa
            - https://www.serebii.net/games/stats.shtml
        - Efetividade de tipagem
        - STAB (Same Type Attack Bonus)
*/
int modificadores(Pokemon *ataca, Pokemon *recebe, int indice);
/*
    Retorna quem vai atacar quem primeiro, em caso de empate, a decisão será randômica
*/
int prioridade_por_velocidade(short int a, short int b);

/*
    
*/
int calcula_super_efetividade(short int type_ataca, Pokemon *recebe);

/*
    Calcula o multiplicador de quão efetivo é o move contra o adversário
*/
int super_efetividade_monotype(short int type_ataca, short int type_defende);

/*  
    Calcula se o movimento acertou
*/
int acertou_movimento(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log);


//faz o calculo sem modificadores do dano final total dado
int calc_dano_final(int dano_bruto, int stat_ofensivo, int stat_defensivo);

//retira as condições de moves
void retirar_move_condition(Pokemon *poke, MCondition condition, int i);

//verifica se é possível atacar pokemon adversário
int consegue_atacar(Pokemon *ataca, char *log);

#endif // UTILIDADES_H
