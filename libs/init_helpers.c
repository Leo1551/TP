#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "headers/pokemon.h"
#include "utilidades.h"

char* init_bst(char *nome){

    FILE *arq = achar_string_em_arquivo(nome, "arquivos/pokemons.txt");
    if (!arq) return NULL;
    
    char *buffer = malloc(100 * sizeof(char));
    if (!buffer) return NULL;

    // passa reto pelo typing
    fscanf(arq, "%99[^ ]", buffer);
    
    //captura base_stats
    fscanf(arq, "%99[^\n]", buffer);
    
    fclose(arq);
    return buffer;
}



Move* init_moves(char *moves_str){

    substituir_espaco_por_underline(moves_str);
    Move *moves = malloc(4 * sizeof(Move));
    if (!moves) return NULL;
    
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

        fscanf(arq, "%d %d", &moves[i].type, &moves[i].categoria);
        fscanf(arq, "%d %d", &moves[i].base_dmg, &moves[i].base_acc);
        fscanf(arq, "%s %d", funcao_move, &target);
        
        moves[i].nome = strdup(move_str[i]);
        moves[i].funcao_move = strdup(funcao_move);
        moves[i].target = target;
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

int* init_monotype(const char *tipo){
    FILE* arq = achar_string_em_arquivo(tipo, "arquivos/types.txt");
    int *i = malloc(sizeof(int));
    if (!i) return NULL;

    fscanf(arq, "%d", i);
    fclose(arq);
    return i;
}

int* init_dualtype(const char *tipo1, const char *tipo2){
    int *dualtype = malloc(2 * sizeof(int*));
    if (!dualtype) return NULL;

    int *type1 = init_monotype((char *)tipo1);
    int *type2 = init_monotype((char *)tipo2);
    
    dualtype[0] = *type1;
    dualtype[1] = *type2;
    
    free(type1);
    free(type2);

    return dualtype;
}

int* init_stats(int max_por_stat, int max_total){
    int *stat = malloc(6 * sizeof(int));
    if (!stat) return NULL;

    const char *names[6] = {"HP", "Atk", "Def", "Spa", "Spd", "Speed"};
   
    for (int i = 0; i < 6; i++){

        do{//capts stat a stat, tendo certeza de que está no limite de upper bound e lower bound
            printf("\nQuantidade restante para distribuir: %d\n", max_total);
            printf("\tInsira %s: ", names[i]);
            if (scanf("%d", &stat[i]) != 1) 
                stat[i] = 0;
        } while (stat[i] < 0 || stat[i] > max_por_stat || stat[i] > max_total); 
        // stat não pode ser menor que zero, não pode ser maior que o upper bound e não pode ultrapassar o máximo que pode ser distribuido no total
        
        max_total -= stat[i];
    }

    return stat;
}