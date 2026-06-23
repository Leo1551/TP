#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/moves.h"
#include "headers/utilidades.h"
#include "headers/pokemon.h"

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
    if (str != NULL)
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

    if (!consegue_atacar(ataca, log))
        return;
    
    if (!acertou_movimento(ataca, recebe, indice_move, log))    
        return;
    
    int *dano_bruto = malloc(sizeof(int)); // necessário para effect_calc
    *dano_bruto = 0;
    
    if (ataca->moves[indice_move].categoria != STATUS)   
        dmg_calc(ataca, recebe, indice_move, log, dano_bruto);
    
    if (recebe->actual_stats.base_hp > 0)
        cause_move_effect(ataca, recebe, indice_move, log, *dano_bruto);
    free(dano_bruto);
}

int acertou_movimento(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log){

    int chance_final = calcular_nivel_multiplicador_accuracy(ataca->multi.m_acc) * ataca->moves[indice_move].base_acc * calcular_nivel_multiplicador_evasion(recebe->multi.m_evasion);
    
    for (int i = 0; i < (sizeof(recebe->moveCondition) / sizeof(MoveCondition)); i++)
        if (recebe->moveCondition[i].condition == FLYING, recebe->moveCondition[i].condition == DIG, recebe->moveCondition[i].condition == DIVE)
        {
            strcat(log, recebe->nome);
            strcat(log, " desviou do movimento adversário\n");
            return 0;
        }
        
    return ((100 * rand())/RAND_MAX < chance_final) ? 1 : 0;
}

void dmg_calc(Pokemon *ataca, Pokemon *recebe, int indice, char *log, int *dano_total){

    int dano = ataca->moves->base_dmg;

    if (ataca->moves[indice].nome == "Fly")
        
    


    if (ataca->moves[indice].categoria == SPECIAL)
    {
        int special_attack = ataca->actual_stats.base_spa;
        int special_defense = recebe->actual_stats.base_spd;

        
        *dano_total = (int) modificadores(ataca, recebe, indice) * (2 + (42 * (dano * special_attack/special_defense)/50));
    
    }
    else
    {
        int attack = ataca->actual_stats.base_atk;
        int defense = recebe->actual_stats.base_def;
        
        *dano_total = (int) modificadores(ataca, recebe, indice) * (2 + (42 * (dano * attack/defense)/50));
    }
    
    // 
    strcat(log, "The opposing used ");
    strcat(log, ataca->moves[indice].nome);
    strcat(log, "!\n (");
    strcat(log, recebe->nome);
    strcat(log, " lost ");

    // no caso do dano recebido ser maior que o hp, ele não fica com hp negativo
    double porcentagem_perdida = ((*dano_total > recebe->actual_stats.base_hp) ? (recebe->actual_stats.base_hp) : (*dano_total) / recebe->base_stats.base_hp);

    char porcentagem_str[32];
    sprintf(porcentagem_str, "%.2f", porcentagem_perdida);
    strcat(log, porcentagem_str);
    strcat(log, "%% of its health!)\n\n");
    //

    recebe->actual_stats.base_hp -= *dano_total;

}

void effect_calc(Pokemon *ataca, Pokemon *recebe, char *str_effect, char *log){
    // TODO
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

int calcula_super_efetividade(short int type_ataca, Pokemon *recebe){

    if (recebe->types[0] == recebe->types[1])
        return super_efetividade_monotype(type_ataca, recebe->types[0]);
    
    return super_efetividade_monotype(type_ataca, recebe->types[0]) * super_efetividade_monotype(type_ataca, recebe->types[1]);

}

int super_efetividade_monotype(short int type_ataca, short int type_defende){
    
    int multiplicador = 1; // dano neutro

    char ataca_str[10], defende_str[10];
    sprintf(ataca_str, "%d", type_ataca);
    sprintf(defende_str, "%d", type_defende);
    
    FILE *arq = fopen("arquivos/types.txt", "r");
    if (!arq) return 1;
    
    char buffer[200];
    int tipo_lido;

    // Verifica as propriedades defensivas do type_defende
    while (fgets(buffer, sizeof(buffer), arq)) {
        if (sscanf(buffer, "%d", &tipo_lido) == 1 && tipo_lido == type_defende) {
            // Encontrou o tipo defensor, verifica Weaknesses, Resistances, Immunities
            while (fgets(buffer, sizeof(buffer), arq)) {
                if (isdigit(buffer[0])) break; // Próximo tipo começou, não altera nada.
                
                if (strstr(buffer, "Weaknesses:")) {
                    if (strstr(buffer, ataca_str)) {
                        fclose(arq);
                        return multiplicador *= 2; // Super efetivo
                    }
                }
                else if (strstr(buffer, "Resistances:")) {
                    if (strstr(buffer, ataca_str)) {
                        fclose(arq);
                        return multiplicador *= 0.5; // Resistência (0.5x damage)
                    }
                }
                else if (strstr(buffer, "Immunities:")) {
                    if (strstr(buffer, ataca_str)) {
                        fclose(arq);
                        return 0; // Imunidade
                    }
                    break; // Fim das propriedades defensivas
                }
            }
            break; // O tipo não foi encontrado
        }
    }
    
    fclose(arq);
    return multiplicador;
}


int prioridade_por_velocidade(short int a, short int b){
    if (a == b) // speed tie gera uma condição randomica de quem vai primeiro
        return (rand() % 2) + 1;
    
    return (a > b) ? 1 : 2;
}

int consegue_atacar(Pokemon *ataca, char *log){

    int qtd_condicoes = sizeof(ataca->moveCondition) / sizeof(MoveCondition);

    for (int i = 0; i < qtd_condicoes; i++)
        switch (ataca->moveCondition[i].condition)
        {
        case IN_CHARGE:
            strcat(log, ataca->nome);
            strcat(log, " está carregando o movimento!\n");
            if (ataca->moveCondition[i].turnos == 1)
                retirar_move_condition(ataca, i);
            else ataca->moveCondition[i].turnos--;
            return 0;
        case BLASTED:
            strcat(log, ataca->nome);
            strcat(log, " está cansado e não consegue atacar!\n");
            if (ataca->moveCondition[i].turnos == 1)
                retirar_move_condition(ataca, i);
            else ataca->moveCondition[i].turnos--;
            return 1;
        case FLYING:
            retirar_move_condition(ataca, i);
            return 1;
        case DIVE:
            retirar_move_condition(ataca, i);
            return 1;
        case BIDE:
            strcat(log, ataca->nome);
            sprintf(log, "está se preparando para o contra-ataque (turnos restantes: %d)", ataca->moveCondition->turnos);
            if (ataca->moveCondition[i].turnos == 1)
                retirar_move_condition(ataca, i);
            else ataca->moveCondition[i].turnos--;
        default:
            continue;
        }
    
    return 1;
}