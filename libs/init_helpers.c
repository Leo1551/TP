#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "headers/pokemon.h"
#include "utilidades.h"

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

    substituir_espaco_por_underline(moves_str);
    Move *moves = malloc(4 * sizeof(Move));
    char move_str[4][100];
    char funcao_move[100];
    int target;
    sscanf(moves_str, "%[^/] %[^/] %[^/] %[^\n]", move_str[0], move_str[1], move_str[2], move_str[3]);


    for (int i = 0; i < 4; i++){
        FILE *arq = achar_string_em_arquivo(move_str[i], "arquivos/moves.txt");
        
        if (!arq){
            move_nao_encontrado_exception(move_str[i], "arquivos/moves.txt");
            return NULL;
        }

        moves[i].nome = move_str[i];

        fscanf(arq, "%d %d", &moves[i].type, &moves[i].categoria);
        fscanf(arq, "%d %d", &moves[i].base_dmg, &moves[i].base_acc);
        fscanf(arq, "%s %d", &funcao_move, &target);

        moves[i].funcao_move = init_move_effect(funcao_move, target);
        moves[i].blocked_turns = 0;
        fclose(arq);
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

Efeito init_move_effect(char *funcao_move, int target){
    
    if (funcao_move == "None")
        return NULL;
    if ()
    {
        /* code */
    }
    
    
}