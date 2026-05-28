#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <headers/erro.h>
#include <headers/pokemon.h>

// Types and prototypes are declared in headers/pokemon.h
static int* str_stats_to_int_array(char *str){
    int *valores = malloc(6 * sizeof(int));
    if (!valores) return NULL;
    for (int k = 0; k < 6; ++k) valores[k] = 0;

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
        .base_stats      = {bst ? bst[0] : 0, bst ? bst[1] : 0, bst ? bst[2] : 0, bst ? bst[3] : 0, bst ? bst[4] : 0, bst ? bst[5] : 0},
        .evs             = {ev_arr ? ev_arr[0] : 0, ev_arr ? ev_arr[1] : 0, ev_arr ? ev_arr[2] : 0, ev_arr ? ev_arr[3] : 0, ev_arr ? ev_arr[4] : 0, ev_arr ? ev_arr[5] : 0},
        .ivs             = {iv_arr ? iv_arr[0] : 0, iv_arr ? iv_arr[1] : 0, iv_arr ? iv_arr[2] : 0, iv_arr ? iv_arr[3] : 0, iv_arr ? iv_arr[4] : 0, iv_arr ? iv_arr[5] : 0},
        .moves           = {move ? move[0] : (Move){0}, move ? move[1] : (Move){0}, move ? move[2] : (Move){0}, move ? move[3] : (Move){0}},
        .statusCondition = OK,
        .types           = {types ? types[0] : 0, types ? types[1] : 0},
        .multi           = {1, 1, 1, 1, 1, 1}
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

