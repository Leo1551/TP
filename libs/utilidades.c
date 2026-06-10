#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "moves.h"
#include "utilidades.h"
#include "pokemon.h"

void limpar_buffer_de_teclado(){
    short int c; while ((c = getchar()) != '\n' && c != EOF);
}

char* show_status_condition(SCondition condition){
    if (condition == OK)
        return "Sem status";
    if (condition == BURN)
        return "Burned";
    if (condition == FREEZE)
        return "Frozen";
    if (condition == PARALYZE)
        return "Paralyzed";
    if (condition == POISON)
        return "Poisoned";
    if (condition == BADLY_POISON)
        return "Badly Poisoned";
    if (condition == SLEEP)
        return "Sleeping";
    if (condition == CONFUSED)
        return "Confused";
    
    return "Não pode ser computado";    
}

char* int_type_to_string(int type){
    char tipo_convertido;
    char *tipo_search;
    sprintf(tipo_convertido, "%c", type);
    FILE *arq = achar_string_em_arquivo(&tipo_convertido, "arquivos/types.txt");

    fscanf(arq, "%s", tipo_search);

    return tipo_search;

}

char* int_category_to_string(int categoria){
    switch (categoria)
    {
        case 1:  return "Physical";
        case 2:  return "Special";
        case 3:  return "Status";
    }
    return "Categoria não computada";
}

void capitalizarPalavras(char *str) {
    int novaPalavra = 1; // Flag para indicar início de palavra
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            novaPalavra = 1;
        } else {
            if (novaPalavra) { // Capitaliza a primeira letra da palavra
                if (str[i] >= 'a' && str[i] <= 'z')
                    str[i] = str[i] - 32;
                    
                novaPalavra = 0;
            } else // Minúsculas para o restante
                if (str[i] >= 'A' && str[i] <= 'Z') 
                    str[i] = str[i] + 32;
                
            
        }
    }
}

void substituir_espaco_por_underline(char *str){
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == ' ')
            str[i] = '_';
}

void trocar_quebra_de_linha_por_terminador(char *str){
    str[strcspn(str, "\n")] = '\0';
}

FILE* achar_string_em_arquivo(char *nome, char *filename){
    FILE *f = fopen(filename, "r");
    if (!f) arquivo_nao_encontrado_exception(filename);
    
    char buffer[100];

    while (fscanf(f, " %63[^ ]", buffer) == 1)
        if (strcmp(buffer, nome) == 0) 
            return f;
    
    fclose(f);
    string_nao_encontrada_exception(nome, filename);
    return NULL;
}

int calc_bst(int bst, int ev, int iv){// todos os pokes são lv 100
    // esses (int) são pra fazer a função floor
    return 5 + (100 * (int) (((2 * (bst + iv)) + (int) (ev/4) + 5)/100));
}

int calc_hp(int bst, int ev, int iv){// todos os pokes são lv 100   
    // esses (int) são pra fazer a função floor
    return (100 * (int) (((2 * (bst + iv)) + (int) (ev/4) + 100)/100)) + 100 + 10;
}

int search_indice_move(Move *moves, char *move){

    int indice = -1; // caso nenhum move esteja certo, joga o erro -1
    for (int i = 0; i < 4; i++)
        if (strcmp(move, moves[i].nome) == 0)
            indice = i;
        
    return indice;
}

char* other_conditions_to_string(MCondition *conditions){
    int tam = sizeof(conditions)/sizeof(MoveCondition);
    char buffer[1000];

    for (int i = 0; i < tam; i++){
        if (conditions[i] == NONE)
            return "None";
        else if (conditions[i] == IN_CHARGE)
            strcat(buffer, "Carregando move");
        else if(conditions[i] == BLASTED)
            strcat(buffer, "Recarregando energias");
        else if(conditions[i] == FLYING)
            strcat(buffer, "Durante Fly");
        else if(conditions[i] == DIVE)
            strcat(buffer, "Usando Dive");
        else if(conditions[i] == DIG)
            strcat(buffer, "Usando Dig");
        else if(conditions[i] == BIND)
            strcat(buffer, "Sob efeito de Bind");
        else if(conditions[i] == MIST)
            strcat(buffer, "Sob efeito de Mist");
        else if(conditions[i] == BIDE)
            strcat(buffer, "Carregando Bide");
        else if(conditions[i] == SEED)
            strcat(buffer, "Sob efeito de Leech Seed");

        strcat(buffer, " ");
    }

    return buffer;
    
}

void move_calc(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log){


    strcat(log, ataca->nome);
    strcat(log, " usou ");
    strcat(log, ataca->moves[indice_move].nome);
    strcat(log, "!\n");

    if (!acertou_movimento())
    {
        strcat(log, recebe->nome);
        strcat(log, " desviou do movimento adversário\n");
        return;
    }

    if (ataca->moves[indice_move].categoria != STATUS)
    {    
        dmg_calc(ataca, recebe, indice_move, log);
    }

    effect_calc(ataca, recebe, ataca->moves[indice_move].funcao_move, log);
    
}

void dmg_calc(Pokemon *ataca, Pokemon *recebe, int indice, char *log){

    int dano = ataca->moves->base_dmg;

    if (ataca->moves[indice].categoria == SPECIAL)
    {
        int special_attack = ataca->actual_stats.base_spa;
        int special_defense = recebe->actual_stats.base_spd;

        recebe->actual_stats.base_hp -= (int) modificadores(ataca, recebe, indice) * (2 + (42 * (dano * special_attack/special_defense)/50));
    }
    else
    {
        int attack = ataca->actual_stats.base_atk;
        int defense = recebe->actual_stats.base_def;
        
        recebe->actual_stats.base_hp -= (int) modificadores(ataca, recebe, indice) * (2 + (42 * (dano * attack/defense)/50));
    }
    
}

void effect_calc(Pokemon *ataca, Pokemon *recebe, char *str_effect, char *log){

}

int modificadores(Pokemon *ataca, Pokemon *recebe, int indice){

    int multiplicador = 1;

    //STAB
    if (is_stab(ataca->types[0], ataca->types[1], ataca->moves[indice].type))
        multiplicador *= 1.5;

    //multiplicadores de bst
    if (ataca->moves[indice].categoria == SPECIAL)
    {
        multiplicador *= calcular_nivel_multiplicador(ataca->multi.m_spa);
        multiplicador *= calcular_nivel_multiplicador(recebe->multi.m_spd);
    }
    else
    {
        multiplicador *= calcular_nivel_multiplicador(ataca->multi.m_atk);
        multiplicador *= calcular_nivel_multiplicador(recebe->multi.m_def);
    }

    //Critical hit
    if (will_cause_critical(ataca->moveCondition, ataca->moves[indice].funcao_move))
        multiplicador *= 1.5;
    
    multiplicador *= calcula_super_efetividade(ataca->moves[indice].type, recebe->types);
    

    

    return multiplicador;
}

int calcula_super_efetividade();

int prioridade_por_velocidade(short int a, short int b){
    if (a == b) // speed tie gera uma condição randomica de quem vai primeiro
        return (rand() % 2) + 1;
    
    return (a > b) ? 1 : 2;
}