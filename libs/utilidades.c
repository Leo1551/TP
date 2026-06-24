#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/moves.h"
#include "headers/utilidades.h"
#include "headers/pokemon.h"
#include "headers/erro.h"

void limpar_buffer_de_teclado(){
    short int c; while ((c = getchar()) != '\n' && c != EOF);
}

char* show_status_condition(SCondition condition){

    switch (condition)
    {
    case           OK:  return "Sem status";
    case         BURN:  return "Burned";
    case       FREEZE:  return "Frozen";
    case     PARALYZE:  return "Paralyzed";
    case       POISON:  return "Poisoned";
    case BADLY_POISON:  return "Badly Poisoned";
    case        SLEEP:  return "Sleeping";
    default:            return "Confused";
    }
}

char* int_type_to_string(int type){
    char *tipo_convertido = malloc(sizeof(char) * 3);
    char *tipo_search = malloc(sizeof(char) * 20);
    sprintf(tipo_convertido, "%s", type);
    FILE *arq = achar_string_em_arquivo(tipo_convertido, "arquivos/types.txt");

    fscanf(arq, "%2s", tipo_search);
    
    free(tipo_convertido);
    fclose(arq);


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

FILE* achar_string_em_arquivo(const char *nome, char *filename){
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
    char buffer = malloc(sizeof(char) * 100);

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
    
    int dano_bruto; // necessário para effect_calc
    dano_bruto = 0;
    
    if (ataca->moves[indice_move].categoria != STATUS)   
        dmg_calc(ataca, recebe, indice_move, log, &dano_bruto);
    
    if (recebe->actual_stats.base_hp > 0)
        cause_move_effect(ataca, recebe, log, indice_move, dano_bruto);

    free(dano_bruto);
}

int acertou_movimento(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log){

    int chance_final = ataca->moves[indice_move].base_acc * calcular_nivel_multiplicador_accuracy(ataca->multi.m_acc) * calcular_nivel_multiplicador_evasion(recebe->multi.m_evasion);
    
    if (ataca->gambiarra_confusion == 1){
        if(!acerta(50))
            {
                int dano = (int) modificadores(ataca, ataca, 1) * calc_dano_final(40, ataca->actual_stats.base_atk, ataca->actual_stats.base_def);
                ataca->actual_stats.base_hp -= dano;
                
                sprintf(log, "%s se machucou devido a Confusão e perdeu %.2f%% (%d/%d) do HP!\n\n", ataca->nome, (float) (dano/ataca->base_stats.base_hp), dano, ataca->base_stats.base_hp);
                return 0;
        }
        else{
            sprintf("%s se curou da confusao!\n", ataca->nome);
            ataca->gambiarra_confusion = 0;
        } 

    }
    for (int i = 0; i < recebe->tam_move_conditions; i++)
        if (recebe->moveCondition[i].condition == FLYING || recebe->moveCondition[i].condition == DIG || recebe->moveCondition[i].condition == DIVE)
        {
            strcat(log, recebe->nome);
            strcat(log, " desviou do movimento adversário\n");
            return 0;
        }

    if (strstr(ataca->moves[indice_move].funcao_move, "Crash")){

        
        if (ataca->base_stats.base_hp/2 > ataca->actual_stats.base_hp){
            sprintf(log, "%s errou o movimento e recebeu perdeu %.2f%% (%d/%d) de HP!\n\n", ataca->nome, (float)(ataca->actual_stats.base_hp/ataca->base_stats.base_hp), ataca->actual_stats.base_hp, ataca->base_stats.base_hp);
            ataca->actual_stats.base_hp = 0;
        }

        int porcentagem_perdida = ataca->actual_stats.base_hp / ataca->base_stats.base_hp/2;
        sprintf(log, "%s errou o movimento e recebeu perdeu %.2f%% (%d/%d) de HP!\n\n", ataca->nome, porcentagem_perdida, ataca->base_stats.base_hp/2, ataca->base_stats.base_hp);

        ataca->actual_stats.base_hp -= ataca->base_stats.base_hp/2;

        return 0;
    }
        
    
    /*  não lembro pra que eu fiz isso, mas se eu descobrir e tiver as palavras-chave: (Fly, Dig, Dive, Missed), tá salvo
    if (recebe->moves[indice_move].nome == "Fly" || recebe->moves[indice_move].nome == "Dig" ||recebe->moves[indice_move].nome == "Dive"){
    
        strcat(log, "The opposing used ");
        strcat(log, ataca->moves[indice_move].nome);
        strcat(log, "!\n (");
        strcat(log, " but missed!");    
    
        return 0
    }
    */   
    return ((100 * rand())/RAND_MAX < chance_final) ? 1 : 0;
}

void dmg_calc(Pokemon *ataca, Pokemon *recebe, int indice, char *log, int *dano_total){

    int dano = ataca->moves->base_dmg;


    if (ataca->moves[indice].categoria == SPECIAL)
    {
        int special_attack = ataca->actual_stats.base_spa;
        int special_defense = recebe->actual_stats.base_spd;

        
        *dano_total = (int) modificadores(ataca, recebe, indice) * calc_dano_final(dano, special_attack, special_defense);
    
    }
    else
    {
        int attack = ataca->actual_stats.base_atk;
        int defense = recebe->actual_stats.base_def;
        
        *dano_total = (int) modificadores(ataca, recebe, indice) * calc_dano_final(dano, attack, defense);
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

int calc_dano_final(int dano_bruto, int stat_ofensivo, int stat_defensivo){
    return 2 + (42 * (dano_bruto * stat_defensivo/stat_defensivo)/50);
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
    
    multiplicador *= calcula_super_efetividade(ataca->moves[indice].type, recebe);
    

    

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
    int consegue = 1;
    switch (ataca->statusCondition.condition)
    {
    case OK: break;
    case FREEZE:
        if (acerta(20)){
            sprintf(log, "%s se descongelou!\n", ataca->nome);
            ataca->statusCondition.condition = OK;
            ataca->statusCondition.turnos = 0;
        }
        else{
            sprintf(log, "%s está congelado!\n", ataca->nome);
            consegue = 0;
        }   
    case SLEEP:
        if (acerta(25)){
            sprintf(log, "%s despertou!\n", ataca->nome);
            ataca->statusCondition.condition = OK;
            ataca->statusCondition.turnos = 0;
        }
        else{
            sprintf(log, "%s está dormindo!\n", ataca->nome);
            consegue = 0;
        }   
    
    default:
        break;
    }

    for (int i = 0; i < ataca->tam_move_conditions; i++){
        switch (ataca->moveCondition[i].condition)
        {
        case IN_CHARGE:

            if (consegue == 1){
                strcat(log, ataca->nome);
                strcat(log, " está carregando o movimento!\n"); 
            }
                
            if (ataca->moveCondition[i].turnos < 1){
                retirar_move_condition(ataca, IN_CHARGE, i);
                return 1;
            }
                
            ataca->moveCondition[i].turnos--;
            return 0;
        case BLASTED:

            if (consegue == 1){
                strcat(log, ataca->nome);
                strcat(log, " está cansado e não consegue atacar!\n");
            }
            
            
            if (ataca->moveCondition[i].turnos < 1){
                retirar_move_condition(ataca, BLASTED, i);
                return 1;
            }

            ataca->moveCondition[i].turnos--;
            return 1;
        case FLYING:
            retirar_move_condition(ataca, FLYING, i);
            return 1;
        case DIVE:
            retirar_move_condition(ataca, DIVE, i);
            return 1;
        case BIDE:
            strcat(log, ataca->nome);
            sprintf(log, "está se preparando para o contra-ataque (turnos restantes: %d)", ataca->moveCondition->turnos);
            if (ataca->moveCondition[i].turnos == 1)
                retirar_move_condition(ataca, BIDE, i);
            else ataca->moveCondition[i].turnos--;
        case FLINCH: 
            if (consegue == 1){            
            strcat(log, "O oponente ");
            strcat(log, ataca->nome);
            strcat(log, " hesitou e não pode se mover!\n");
            }
            retirar_move_condition(ataca, FLINCH, i);
        default:
            continue;
        }
    }

    return 1;
}

int acerta(int chance_base_acerto){
    return ((double) rand()/RAND_MAX < chance_base_acerto/100) ? 1 : 0;
}


// o único problema disso é que ele pode chegar no tamanho máximo de condições = menos de 10
void retirar_move_condition(Pokemon *poke, MCondition condition, int i){
    poke->moveCondition[i].condition = NONE;
}
