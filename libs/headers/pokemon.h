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
}PokemonBaseStats;

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
    int m_atk;
    int m_def;
    int m_spa;
    int m_spd;
    int m_spe;
    int m_acc;
    int m_evasion;
}PokemonMultiplicadores; // 6 campos, todos devem variar de [-6 6]

typedef enum{
    PHYSICAL = 1,
    SPECIAL = 2,
    STATUS = 3
}Categoria;

/*
    Cada move terá suas específicações guardadas em um arquivo *possivelmente moves.txt*

    Cada move será inicializado por uma função Move init_move(String nome)

*/
typedef struct{
    char *nome;
    short int type;
    Categoria categoria; //physical = 1, special = 2, status = 3
    short int base_dmg;
    short int base_acc;
    short int blocked_turns; // 0 se não estiver bloqueado, causado por taunt ou disable
    char *funcao_move;// recebe o pipe com as funções que devem ser executadas em uma possível função cause_effect(Pokemon atk, Pokemon recebe, int target)
    short int target;
}Move;


typedef enum{
    OK = 0,
    BURN = 1,           // dano *= 1/2
    FREEZE = 2,         // Condição pré-ataque (20% de virar OK)
    PARALYZE = 3,       // Condição pré-ataque (25% de impedir uso de move) + (Speed *= 1/2)
    POISON = 4,         // hp -= (max_hp * 7/8) 
    BADLY_POISON = 5,   // hp -= (max_hp * 1/16 * (qtd_turnos)) 
    SLEEP = 6,          //
    RESTING = 7
}SCondition;

typedef struct {
    SCondition condition;
    unsigned short int turnos;
}StatusCondition;

typedef enum{
    NONE,
    IN_CHARGE, // condição pré-ataque
    BLASTED, // condição pré-ataque
    FLYING, // condição pré-ataque
    DIVE, // condição pré-ataque
    DIG, // condição pré-ataque
    BIND,    //condição pós-turno
    MIST, // condição pré-ataque
    BIDE, //condição pós-turno
    SEED, //condição pós-turno
    CRIT, // condição pré-ataque
    FLINCH
}MCondition;

typedef struct{
    MCondition condition;
    unsigned short int turnos;
    unsigned short int bide_total_dmg; // apagar isso quando tiver uma ideia melhor de como lidar com Bide
}MoveCondition;

typedef struct{
    char *nome;
    PokemonBaseStats base_stats;
    PokemonStats actual_stats;
    PokemonMultiplicadores multi;
    StatusCondition statusCondition;
    Move moves[4];
    MoveCondition *moveCondition;
    short int tam_move_conditions;
    short int types[2];
    short int gambiarra_confusion;
}Pokemon;

Pokemon init_pokemon(char *nome, int *evs, int *ivs, char *moves);
int* str_stats_to_int_array(char *str);
void show_info(Pokemon pokemon);

/*
    Os três servem para lidar com os multiplicadores de base stats (BST)
*/
float calcular_nivel_multiplicador(int nivel);
float calcular_nivel_multiplicador_accuracy(int nivel);
float calcular_nivel_multiplicador_evasion(int nivel);


char* init_bst(char *nome);
Move* init_moves(char *moves);
int*  init_types(char *nome);
char* int_type_to_string(int type);
int   calc_hp(int base, int ev, int iv);
int   calc_bst(int base, int ev, int iv);
char* show_status_condition(int condition);
char* other_conditions_to_string(MoveCondition *moveCondition);
char* int_category_to_string(int categoria);
#endif // POKEMON_H