#include "pokemon.h"

#ifndef MOVES_H
#define MOVES_H

//todas os efeitos terão cause no início
//todos os efeitos seguirão as leis da gen 6

//moves de (force switch, weight damage, priority, screens, conversion,mirrior move, transform) não serão implementados


void cause_move_effect(Pokemon *ataca, Pokemon *recebe, char *log, int indice_move, int dano_bruto);

void cause_badly_poison(Pokemon *poke);

/*
    Essa função precisa calcular a chance de tal poke receber um Status Condition (definido em StatusCondition em pokemon.h)
    Isso levará em conta apenas a chance do efeito do move ser aplicado
*/
void cause_status_condition(Pokemon *pokemon, SCondition tipo, float chance_base_acerto, char *log);

/*
    Ex: Dig, Dive, Fly, LeechSeed, bla, bla bla
    quando turnos = -1, a condição é infinita
*/
void cause_move_condition(Pokemon *pokemon, MoveCondition condicao, int turnos);

/*
    Vai botar StatusCondition = OK
*/
void cause_retrive_status_condition(Pokemon *pokemon);


/*

*/
void cause_recoil_dmg(Pokemon *pokemon, double dano_total_ao_adversario, double porcentagem_recoil);

//ainda definirei como será feito o sistema de Multi-Hit moves
/*
Number of hits	Chance of happening
Gen V on
2 7/20 (35%)
3 7/20 (35%)
4 3/20 (15%)
5 3/20 (15%)
*/
int quantidade_hits_acertados();

/*
    É aqui onde será settado as condições onde haverá charging waiting ou coisas do tipo
    Talvez eu tenha que segurar aqui qual move deverá ser usado, pois quando acabar o turno de carga, eu devo colocar o move com dano + efeito do move (Razor Wind)
*/
void cause_turn_wait(Pokemon *pokemon, MoveCondition condition);


/*
    Isso vai levar em conta se o move tem alguma extensão que faça com que dê mais chances de critical hit

    https://bulbapedia.bulbagarden.net/wiki/Critical_hit#Probability_2
*/
int will_cause_critical(MoveCondition *conditions, char *effect, int indice);

/*

*/
int will_cause_flinch(int porcentagem);

/*

*/
int will_cause_confusion(Pokemon *pokemon, int chance);

/*
    (int tipo) -> 1 = atk, 2 = def, 3 = spa, 4 = spd, 5 = spe, 6 = accuracy, 7 = evasion
    (int modificador) -> https://bulbapedia.bulbagarden.net/wiki/Stat_modifier
*/
void cause_status_multiplier_change(Pokemon *pokemon, int tipo, int nivel_modificador);


/*
    Não está na forma final, confira: https://www.smogon.com/dex/ss/moves/bind/

*/
void cause_bind(Pokemon *adversario, int i);


/*
    Quando o move tem chance infinita de acertar, quando acerta o primeiro hit
*/
void cause_hit_again(Pokemon *ataca, Pokemon *recebe, int indice_move, int many_times);

/*
    Ex: Jump Kick

    LEMBRAR QUE SE DÁ FLINCH UMA VEZ, ELE TEM QUE IMPEDIR QUE CAUSE DUAS VEZES
*/
void cause_crash(Pokemon *pokemon, int porcentagem);

/*
    Ex: Sonicboom, Dragon Rage
*/
void cause_fixed_dmg(Pokemon *inimigo, int dano);


/*

*/
void cause_disable(Pokemon *inimigo);

/*

*/
void cause_taunt(Pokemon *inimigo);

/*

*/
void cause_drain(Pokemon *recebe, int dano_bruto, int porcentagem);

/*

*/
void cause_leechseed(Pokemon *recebe, Pokemon *perde);

/*
    Solar Beam
*/
void cause_charging_move(Pokemon *ataca);

/*
    Ex: Thrash, Petal Dance 
*/
void cause_consecutive_turns_move();

/*

*/
void cause_mimic(Pokemon *recebe, Pokemon *copiado);

/*
    Rest, Recover
*/
void recovery(Pokemon *recebe, int porcentagem);
void psywave(Pokemon *recebe);
void superfang(Pokemon *recebe);
void bide(Pokemon *pokemon);
void cause_2_5_multihit(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log);


int is_stab(int tipo1, int tipo2, int tipo_move);


int* valores_multiplos_em_parenteses(char *ptr, int qtd);
int valores_em_parenteses(char *ptr);   
int acerta(double chance);
void cause_confusion(Pokemon *pokemon);
void add_move_condition(Pokemon *pokemon, MoveCondition new_condition);
void cause_recharge(Pokemon *pokemon);


#endif