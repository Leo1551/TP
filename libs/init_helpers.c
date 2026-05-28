#include <stdlib.h>
#include <string.h>
#include "headers/pokemon.h"

int* init_bst(char *nome){
    int *arr = malloc(6 * sizeof(int));
    if (!arr) return NULL;
    for (int i = 0; i < 6; ++i) arr[i] = 0;
    return arr;
}

Move* init_moves(char *moves_str){
    Move *moves = malloc(4 * sizeof(Move));
    if (!moves) return NULL;
    for (int i = 0; i < 4; ++i){
        moves[i].type = 0;
        moves[i].categoria = 0;
        moves[i].base_dmg = 0;
        moves[i].base_acc = 0;
        moves[i].funcao = NULL;
    }
    return moves;
}

int* init_types(char *nome){
    int *t = malloc(2 * sizeof(int));
    if (!t) return NULL;
    t[0] = t[1] = 0;
    return t;
}
