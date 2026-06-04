#ifndef POKEMON_H
#define POKEMON_H

#include <string.h>
#include <stdlib.h>

typedef struct{
    unsigned short int base_hp;
    unsigned short int base_atk;
    unsigned short int base_def;
    unsigned short int base_spa;
    unsigned short int base_spd;
    unsigned short int base_spe;
}PokemonStats;

typedef struct{
    unsigned short int ivs_hp;
    unsigned short int ivs_atk;
    unsigned short int ivs_def;
    unsigned short int ivs_spa;
    unsigned short int ivs_spd;
    unsigned short int ivs_spe;
}PokemonIVs;// 6 campos + maxEvs

typedef struct{
    unsigned short int evs_hp;
    unsigned short int evs_atk;
    unsigned short int evs_def;
    unsigned short int evs_spa;
    unsigned short int evs_spd;
    unsigned short int evs_spe;
}PokemonEVs; // 6 campos + maxEvs


typedef struct{
    float m_atk;
    float m_def;
    float m_spa;
    float m_spd;
    float m_spe;
    float m_acc;
    float m_evasion;

}PokemonMultiplicadores; // 6 campos


// este typedef serve para que seja feito um ponteiro para todas as funções que terão algum efeito
typedef int (*Efeito)(Pokemon, Pokemon, int);

/*
    Cada move terá suas específicações guardadas em um arquivo *possivelmente moves.txt*

    Cada move será inicializado por uma função Move init_move(String nome)

*/
typedef struct{
    char *nome;
    short int type;
    short int categoria; //physical = 1, special = 2, status = 3
    short int base_dmg;
    short int base_acc;
    short int blocked_turns; // 0 se não estiver bloqueado, causado por taunt ou disable
    Efeito funcao_move;// int de target 0 = self, 1 = adjacent
}Move;


typedef enum{
    OK = 0,
    BURN = 1,           // dano *= 1/2
    FREEZE = 2,         // Condição pré-ataque (20% de virar OK)
    PARALYZE = 3,       // Condição pré-ataque (25% de impedir uso de move) + (Speed *= 1/2)
    POISON = 4,         // hp -= (max_hp * 7/8) 
    BADLY_POISON = 5,   // hp -= (max_hp * 1/16 * (qtd_turnos)) 
    SLEEP = 6,          //
    RESTING = 7,
    CONFUSED = 8 
}StatusCondition;

typedef enum{
    NONE,
    IN_CHARGE,
    BLASTED, 
    FLYING, 
    DIVE,
    DIG,
    BIND,    //condição pós-turno
    MIST,
    BIDE,
    SEED
}MoveCondition;

typedef struct{
    PokemonStats base_stats;
    PokemonMultiplicadores multi;
    StatusCondition statusCondition;
    Move moves[4];
    MoveCondition moveCondition;
    short int types[2];
}Pokemon;

#endif // POKEMON_H