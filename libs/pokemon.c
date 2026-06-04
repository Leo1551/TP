#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <headers/erro.h>
#include <headers/pokemon.h>

static int* str_stats_to_int_array(char *str){
    
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

Pokemon init_pokemon(char *nome, char *evs, char *ivs, char *moves){
    
    int *bst = str_stats_to_int_array(init_bst(nome));
    if (bst == NULL)
        nome_pokemon_exception(nome);
    
    int *ev_arr = str_stats_to_int_array(evs);

    int *iv_arr = str_stats_to_int_array(ivs);

    Move *move = init_moves(moves);

    int *types = init_types(nome);
    if (types == NULL)
        tipos_exception(nome);
    

    Pokemon pokemon = {
        .base_stats      = {
                                calc_hp (bst[0], ev_arr[0], iv_arr[0]),  // hp
                                calc_bst(bst[1], ev_arr[1], iv_arr[1]), // atk
                                calc_bst(bst[2], ev_arr[2], iv_arr[2]), // def
                                calc_bst(bst[3], ev_arr[3], iv_arr[3]), // spa
                                calc_bst(bst[4], ev_arr[4], iv_arr[4]), // spd
                                calc_bst(bst[5], ev_arr[5], iv_arr[5])  // spe
                            },
        .moves           = {
                                move ? move[0] : (Move){NULL}, 
                                move ? move[1] : (Move){NULL}, 
                                move ? move[2] : (Move){NULL}, 
                                move ? move[3] : (Move){NULL}
                            },
        .types           = {
                            types ? types[0] : 1, 
                            types ? types[1] : 1
                            },
        .multi           = {1, 1, 1, 1, 1, 1},
        .statusCondition = OK,
        .moveCondition   = NONE
    };

    free(bst);
    free(ev_arr);
    free(iv_arr);
    free(move);
    free(types);

    return pokemon;
}


void show_info(Pokemon){

}

