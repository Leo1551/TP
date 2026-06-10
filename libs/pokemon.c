#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <headers/erro.h>
#include <headers/pokemon.h>

int* str_stats_to_int_array(char *str){
    
    int *valores = calloc(6, sizeof(int));
    
    if (!valores) return NULL;  
    if (str == NULL) return valores;

    char *s = strdup(str);
    if (!s) return valores;
    char *token = strtok(s, "/");

    int i = 0;
    while (token != NULL && i < 6) {
        valores[i] = atoi(token); // converte para inteiro
        token = strtok(NULL, "/");
        i++;
    }
    free(s);
    return valores;
}

Pokemon init_pokemon(char *nome, int *evs, int *ivs, char *moves){
    
    int *bst = str_stats_to_int_array(init_bst(nome));
    if (bst == NULL){
        nome_pokemon_exception(nome);
        return (Pokemon){.nome = NULL};
    }
        

    Move *move = init_moves(moves);
    int *types = init_types(nome);

    if (types == NULL)
        tipos_exception(nome);
    

    Pokemon pokemon = {
        .nome = nome,
        .base_stats      = { //esses serão constantes
                                calc_hp (bst[0], evs[0], evs[0]), // hp
                                calc_bst(bst[1], evs[1], evs[1]), // atk
                                calc_bst(bst[2], evs[2], evs[2]), // def
                                calc_bst(bst[3], evs[3], evs[3]), // spa
                                calc_bst(bst[4], evs[4], evs[4]), // spd
                                calc_bst(bst[5], evs[5], evs[5])  // spe
                            },
        .actual_stats    = {    // esses não serão variáveis
                                calc_hp (bst[0], evs[0], evs[0]), // hp
                                calc_bst(bst[1], evs[1], evs[1]), // atk
                                calc_bst(bst[2], evs[2], evs[2]), // def
                                calc_bst(bst[3], evs[3], evs[3]), // spa
                                calc_bst(bst[4], evs[4], evs[4]), // spd
                                calc_bst(bst[5], evs[5], evs[5])  // spe
                            
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
        .multi           = {1, 1, 1, 1, 1, 1},
        .statusCondition = OK,
        .moveCondition   = malloc(sizeof(MoveCondition)) // automaticamente inicializa com NONE
    };

    free(bst);
    free(evs);
    free(evs);
    free(move);
    free(types);

    return pokemon;
}

void show_info(Pokemon pokemon){
    printf("\n\n=================%s=======================\n", pokemon.nome);
    printf("HP: %d/%d\n", pokemon.actual_stats.base_hp, pokemon.base_stats.base_hp);
    printf("Atk: (%d) %d/%d\n", pokemon.multi.m_atk, pokemon.actual_stats.base_atk, pokemon.base_stats.base_atk);
    printf("Atk: (%d) %d/%d\n", pokemon.multi.m_def, pokemon.actual_stats.base_def, pokemon.base_stats.base_def);
    printf("Atk: (%d) %d/%d\n", pokemon.multi.m_spa, pokemon.actual_stats.base_spa, pokemon.base_stats.base_spa);
    printf("Atk: (%d) %d/%d\n", pokemon.multi.m_spd, pokemon.actual_stats.base_spd, pokemon.base_stats.base_spd);
    printf("Atk: (%d) %d/%d\n", pokemon.multi.m_spe, pokemon.actual_stats.base_spe, pokemon.base_stats.base_spe);
    printf("=============================================================\n");
    printf("Status Condition: %s (Turnos: %d)\n", show_status_condition(pokemon.statusCondition.condition), pokemon.statusCondition.turnos);
    printf("Other Conditions: %s", other_conditions_to_string(pokemon.moveCondition));
    printf("========================== Moves ============================\n");
    for (int i = 0; i < 4; i++){
        printf("%s / %s / %s\n", pokemon.moves[i].nome, int_type_to_string(pokemon.moves[i].type), int_category_to_string(pokemon.moves[i].categoria));
        printf("\t dmg: %d\n", pokemon.moves[i].base_dmg);
        printf("\t acc: %d\n", pokemon.moves[i].base_acc);
        printf("\t Função: %s\n", pokemon.moves[i].funcao_move);
        printf("Turnos bloqueados: %d\n\n", pokemon.moves[i].blocked_turns);
    }
    printf("==============================================================\n\n");
}

