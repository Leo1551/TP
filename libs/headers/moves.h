#include "pokemon.h"

//todas os efeitos terão cause no início
//todos os efeitos seguirão as leis da gen 6

//moves de (force switch, weight damage, priority, screens, conversion,mirrior move, transform) não serão implementados


/*
    Essa função precisa calcular a chance de tal poke receber um Status Condition (definido em StatusCondition em pokemon.h)
    Isso levará em conta apenas a chance do efeito do move ser aplicado
*/
void cause_status_condition(Pokemon *pokemon, StatusCondition tipo, float chance_base_acerto);

/*
    Ex: Dig, Dive, Fly, LeechSeed, bla, bla bla
*/
void cause_move_condition(Pokemon *pokemon, MoveCondition condicao);

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
int will_cause_critical(int grau_move);

/*

*/
int will_cause_flinch(int porcentagem);

/*

*/
int will_cause_confusion(Pokemon *pokemon);

/*
    (int tipo) -> 1 = atk, 2 = def, 3 = spa, 4 = spd, 5 = spe, 6 = accuracy, 7 = evasion
    (int modificador) -> https://bulbapedia.bulbagarden.net/wiki/Stat_modifier
*/
void cause_status_multiplier_change(Pokemon pokemon, int tipo, int modificador);


/*
    Não está na forma final, confira: https://www.smogon.com/dex/ss/moves/bind/

*/
void cause_bind(Pokemon adversario);


/*
    Quando o move tem chance infinita de acertar, quando acerta o primeiro hit
*/
void cause_hit_again(int many_times);

/*
    Ex: Jump Kick

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

void psywave();
void superfang();
void bide();
void dream_eater();