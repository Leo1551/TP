#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "headers/pokemon.h"

char* init_bst(char *nome){

    FILE *arq = achar_string_em_arquivo(nome, "arquivos/pokemons.txt");
    if (!arq) return NULL;
    
    char buffer[20];

    // passa reto pelo typing
    fscanf(arq, "%19[^ ]", buffer);
    
    //captura base_stats
    fscanf(arq, "%19[^\n]", buffer);
    

    return buffer;
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
    FILE *arq = achar_string_em_arquivo(nome, "arquivos/pokemons.txt");
    if (!arq) return NULL;

    char tipo1[20] = {0};
    char tipo2[20] = {0};

    // lê tipo1 (pulando espaços) até '/' ou até espaço
    // tenta ler "/tipo2" 
    if ((fscanf(arq, " %20[^/]", tipo1) != 1) || (fscanf(arq, "/%63[^ ]", tipo2) != 1)){
        fclose(arq);
        return NULL;
    }

    fclose(arq);
    
    if (strcmp(tipo1, tipo2) == 0) 
        return init_monotype(tipo1);
    return init_dualtype(tipo1, tipo2);
}

int* init_monotype(char *tipo){
    FILE* arq = achar_string_em_arquivo(tipo, "arquivos/types.txt");
    int *i = malloc(sizeof(int));

    fscanf(arq, "%d", i);
    return i;
}

int* init_dualtype(const char *tipo1, const char *tipo2){
    int *dualtype = malloc(2 * sizeof(int));
    dualtype[0] = init_monotype(tipo1);
    dualtype[1] = init_monotype(tipo2);

    return dualtype;
}

FILE* achar_string_em_arquivo(char *nome, char *filename){
    FILE *f = fopen(filename, "r");
    if (!f) string_nao_encontrada_exception(nome, filename);
    char buffer[64];

    while (fscanf(f, " %63[^ ]", buffer) == 1)
        if (strcmp(buffer, nome) == 0) 
            return f;
    
    fclose(f);

    return NULL;
}