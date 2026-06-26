#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "headers/erro.h"
#include "headers/pokemon.h"
#include "headers/utilidades.h"

int* str_stats_to_int_array(char *str)
{
    if (!str) return NULL;

    int *valores = calloc(6, sizeof(int));
    if (!valores) return NULL;

    char *s = strdup(str);
    if (!s) {
        free(valores);
        return NULL;
    }

    char *token = strtok(s, "/");
    int i = 0;

    while (token != NULL && i < 6){
        valores[i] = atoi(token);
        token = strtok(NULL, "/");
        i++;
    }

    free(s);
    return valores;
}

Pokemon *init_pokemon(char *nome_poke, int *evs, int *ivs, char *moves){
    

    int *bst = str_stats_to_int_array(init_bst(nome_poke));
    
    /* debug
    printf("============================\n\nPokemon: %s\nIVs:(", nome_poke);
    for (int i = 0; i < 6; i++)
        printf("%d/", evs[i]);
    printf(")\nIVs:(");    
    for (int i = 0; i < 6; i++)
        printf("%d/", ivs[i]);
    printf(")\nMoves:%s\n\n=========================\n", moves);

    

    printf("%s base stats:(", nome_poke);
    for(int i = 0; i < 6; i++)
        printf("%d/", bst[i]);
    printf(")\n");
    system("sleep 10");
    */
    

    if (bst == NULL){
        nome_pokemon_exception(nome_poke);
        return &(Pokemon){NULL};
    }

    
    Move *move = init_moves(moves);
    int *types = init_types(nome_poke);

    if (types == NULL)
        tipos_exception(nome_poke);
    
    printf("DEBUG: Tipos carregados para %s: tipo1=%s, tipo2=%s\n", nome_poke, int_type_to_string(types[0]), int_type_to_string(types[1]));

    Pokemon *pokemon = malloc(sizeof(Pokemon));    

    *pokemon = (Pokemon){
        .nome = strdup(nome_poke),
        .base_stats      = { //esses serão constantes
                                calc_hp (bst[0], evs[0], ivs[0]), // hp
                                calc_bst(bst[1], evs[1], ivs[1]), // atk
                                calc_bst(bst[2], evs[2], ivs[2]), // def
                                calc_bst(bst[3], evs[3], ivs[3]), // spa
                                calc_bst(bst[4], evs[4], ivs[4]), // spd
                                calc_bst(bst[5], evs[5], ivs[5])  // spe
                            },
        .actual_stats    = {    // esses não serão variáveis
                                calc_hp (bst[0], evs[0], ivs[0]), // hp
                                calc_bst(bst[1], evs[1], ivs[1]), // atk
                                calc_bst(bst[2], evs[2], ivs[2]), // def
                                calc_bst(bst[3], evs[3], ivs[3]), // spa
                                calc_bst(bst[4], evs[4], ivs[4]), // spd
                                calc_bst(bst[5], evs[5], ivs[5])  // spe
                            
                            },
        .moves           = {    // os moves que não foram settados devem ser nulos para os dialogos fluirem
                                move ? move[0] : (Move){NULL}, 
                                move ? move[1] : (Move){NULL}, 
                                move ? move[2] : (Move){NULL}, 
                                move ? move[3] : (Move){NULL}
                            },
        .types           = { // caso tenha um único type, os dois serão o mesmo
                            types ? types[0] : 1, 
                            types ? types[1] : 1
                            },
        .multi           = {0, 0, 0, 0, 0, 0, 0},
        .gambiarra_confusion = 0,
        .tam_move_conditions = 1,
        .statusCondition = (StatusCondition){.condition = OK,.turnos = 0},
        .moveCondition   = malloc(sizeof(MoveCondition)) // automaticamente inicializa com NONE
    };
    pokemon->moveCondition[0].condition = NONE;
    pokemon->moveCondition[0].turnos = 0;

    validar_pokemon_inicializado(pokemon, pokemon->nome);

    free(bst);
    free(ivs);
    free(evs);
    free(move);
    free(types);

    return pokemon;
}

void show_info(Pokemon pokemon){
    printf("\n\n=================[%s](%s/%s)======================\n", pokemon.nome, int_type_to_string(pokemon.types[0]), int_type_to_string(pokemon.types[1]));
    printf("\tHP: %d/%d\n", pokemon.actual_stats.base_hp, pokemon.base_stats.base_hp);
    printf("\tAtk: (%d) %d/%d\n", pokemon.multi.m_atk, (int) (pokemon.actual_stats.base_atk * calcular_nivel_multiplicador(pokemon.multi.m_atk)), pokemon.base_stats.base_atk);
    printf("\tDef: (%d) %d/%d\n", pokemon.multi.m_def, (int)(pokemon.actual_stats.base_def * calcular_nivel_multiplicador(pokemon.multi.m_def)), pokemon.base_stats.base_def);
    printf("\tSpA: (%d) %d/%d\n", pokemon.multi.m_spa, (int)(pokemon.actual_stats.base_spa * calcular_nivel_multiplicador(pokemon.multi.m_spa)), pokemon.base_stats.base_spa);
    printf("\tSpD: (%d) %d/%d\n", pokemon.multi.m_spd, (int)(pokemon.actual_stats.base_spd * calcular_nivel_multiplicador(pokemon.multi.m_spd)), pokemon.base_stats.base_spd);
    printf("\tSpe: (%d) %d/%d\n", pokemon.multi.m_spe, (int)(pokemon.actual_stats.base_spe * calcular_nivel_multiplicador(pokemon.multi.m_spe)), pokemon.base_stats.base_spe);
    printf("\tAccuracy: %.2fx Evasion: %.2fx\n", calcular_nivel_multiplicador_accuracy(pokemon.multi.m_acc), calcular_nivel_multiplicador_evasion(pokemon.multi.m_evasion));
    printf("=============================================================\n");
    printf("Status Condition: %s (Turnos: %d)\n", show_status_condition(pokemon.statusCondition.condition), pokemon.statusCondition.turnos);
    printf("Other Conditions: %s\n", other_conditions_to_string(pokemon.moveCondition, pokemon.tam_move_conditions));
    printf("========================== Moves ============================\n");
    for (int i = 0; i < 4; i++){
        
        if (pokemon.moves[i].nome == NULL){
            printf("NULL");
            continue;
        }
            
        printf("%s / %s / %s\n", pokemon.moves[i].nome, int_type_to_string(pokemon.moves[i].type), int_category_to_string(pokemon.moves[i].categoria));
        printf("\t dmg: %d\n", pokemon.moves[i].base_dmg);
        printf("\t acc: %.2f%%\n", pokemon.moves[i].base_acc * calcular_nivel_multiplicador_accuracy(pokemon.multi.m_acc));
        printf("\t Função: %s\n", pokemon.moves[i].funcao_move);
        printf("Turnos bloqueados: %d\n", pokemon.moves[i].blocked_turns);
        printf("______________________________________________________________\n\n");
    }
    printf("==============================================================\n\n");
}

float calcular_nivel_multiplicador(int nivel){
    switch (nivel)
    {
    case -6: return 0.25;
    case -5: return 0.285;
    case -4: return 0.33;
    case -3: return 0.4;
    case -2: return 0.5;
    case -1: return 0.66;
    case 0: return 1;
    case 1: return 1.5;
    case 2: return 2;
    case 3: return 2.5;
    case 4: return 3;
    case 5: return 3.5;
    default: return 4;
    }
}

float calcular_nivel_multiplicador_accuracy(int nivel){
    switch (nivel)
    {
    case -6: return 0.33;
    case -5: return 0.375;
    case -4: return 0.428;
    case -3: return 0.5;
    case -2: return 0.6;
    case -1: return 0.75;
    case 0: return 1;
    case 1: return 1.33;
    case 2: return 1.66;
    case 3: return 2;
    case 4: return 2.33;
    case 5: return 2.66;
    default: return 3;
    }
}

float calcular_nivel_multiplicador_evasion(int nivel){
    switch (nivel)
    {
    case -6: return 3;
    case -5: return 2.66;
    case -4: return 2.33;
    case -3: return 2;
    case -2: return 1.66;
    case -1: return 1.33;
    case 0: return 1;
    case 1: return 0.75;
    case 2: return 0.6;
    case 3: return 0.5;
    case 4: return 0.428;
    case 5: return 0.375;
    default: return 0.33;
    }
}

