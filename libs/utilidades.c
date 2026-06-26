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

char *int_type_to_string(int type){
    switch (type)
    {
        case 1: return "Normal";
        case 2: return "Fire";
        case 3: return "Water";
        case 4: return "Grass";
        case 5: return "Electric";
        case 6: return "Ice";
        case 7: return "Fighting";
        case 8: return "Poison";
        case 9: return "Ground";
        case 10: return "Rock";
        case 11: return "Flying";
        case 12: return "Psychic";
        case 13: return "Bug";
        case 14: return "Ghost";
        case 15: return "Steel";
        case 16: return "Dragon";
        case 17: return "Dark";
        case 18: return "Fairy";
        default: return "Unknown";
    }
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
int inicio_palavra = 1;

    while (*str) {
        if (isspace((unsigned char)*str) || *str == '/') {
            inicio_palavra = 1;
        }
        else if (inicio_palavra) {
            *str = toupper((unsigned char)*str);
            inicio_palavra = 0;
        }
        else {
            *str = tolower((unsigned char)*str);
        }

        str++;
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
    if (!f) return NULL;

    size_t len = strlen(nome);

    int c;

    while ((c = fgetc(f)) != EOF)
    {
        if (c == nome[0])
        {
            long pos = ftell(f) - 1;

            int i;
            for (i = 1; i < len; i++)
            {
                c = fgetc(f);
                if (c == EOF || c != nome[i])
                    break;
            }

            if (i == len){
                char c = fgetc(f);
                //volta 0, pois averigou se a string acabou ou nao
                if (!(c == ' ' || c == '\n' || c == '\0'))
                    continue;
                
                
                fseek(f, -1, SEEK_CUR);
                // encontrou a string inteira
                return f; // ponteiro já está logo após a string
            }

            // não era match -> volta para continuar busca corretamente
            fseek(f, pos + 1, SEEK_SET);
        }
    }

    fclose(f);
    return NULL;
}

int calc_bst(int bst, int ev, int iv)
{
    return (2 * bst + iv + ev / 4) + 5;
}
int calc_hp(int bst, int ev, int iv)
{
    return (2 * bst + iv + ev / 4) + 110;
}

int search_indice_move(Move *moves, char *move){

    int indice = -1; // caso nenhum move esteja certo, joga o erro -1
    for (int i = 0; i < 4; i++)
        if (strcmp(move, moves[i].nome) == 0)
            indice = i;
        
    return indice;
}

char* other_conditions_to_string(MoveCondition *conditions, int tam){
    
    char *buffer = malloc(sizeof(char) * 100);

    for (int i = 0; i < tam; i++){
        if (conditions[i].condition == NONE)
            return "None";
        else if (conditions[i].condition == IN_CHARGE)
            strcat(buffer, "Carregando move");
        else if(conditions[i].condition == BLASTED)
            strcat(buffer, "Recarregando energias");
        else if(conditions[i].condition == FLYING)
            strcat(buffer, "Durante Fly");
        else if(conditions[i].condition == DIVE)
            strcat(buffer, "Usando Dive");
        else if(conditions[i].condition == DIG)
            strcat(buffer, "Usando Dig");
        else if(conditions[i].condition == BIND)
            strcat(buffer, "Sob efeito de Bind");
        else if(conditions[i].condition == MIST)
            strcat(buffer, "Sob efeito de Mist");
        else if(conditions[i].condition == BIDE)
            strcat(buffer, "Carregando Bide");
        else if(conditions[i].condition == SEED)
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
    
    int dano_bruto = 0; // necessário para effect_calc
    
    if (ataca->moves[indice_move].categoria != STATUS)   
        dmg_calc(ataca, recebe, indice_move, log, &dano_bruto);
    
    if (recebe->actual_stats.base_hp > 0)
        cause_move_effect(ataca, recebe, log, indice_move, dano_bruto);

}

int acertou_movimento(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log){

    int chance_final = ataca->moves[indice_move].base_acc * calcular_nivel_multiplicador_accuracy(ataca->multi.m_acc) * calcular_nivel_multiplicador_evasion(recebe->multi.m_evasion);
    char *log_acertou = malloc(sizeof(char) * 500);
    if (ataca->gambiarra_confusion == 1){
        if(!acerta(50))
            {
                int dano = (int) modificadores(ataca, ataca, 1) * calc_dano_final(40, ataca->actual_stats.base_atk, ataca->actual_stats.base_def);
                ataca->actual_stats.base_hp -= dano;
                
                sprintf(log_acertou, "%s se machucou devido a Confusão e perdeu %.2f%% (%d/%d) do HP!\n\n", ataca->nome, (float) (dano/ataca->base_stats.base_hp), dano, ataca->base_stats.base_hp);
                return 0;
        }
        else{
            sprintf(log_acertou, "%s se curou da confusao!\n", ataca->nome);
            ataca->gambiarra_confusion = 0;
        } 

    }
    for (int i = 0; i < recebe->tam_move_conditions; i++)
        if (recebe->moveCondition[i].condition == FLYING || recebe->moveCondition[i].condition == DIG || recebe->moveCondition[i].condition == DIVE)
        {
            strcat(log_acertou, recebe->nome);
            strcat(log_acertou, " desviou do movimento adversário\n");
            strcat(log, log_acertou);
            free(log_acertou);
            return 0;
        }

    if (strstr(ataca->moves[indice_move].funcao_move, "Crash")){
    char *log_acertou2 = malloc(sizeof(char) * 200);
        
        if (ataca->base_stats.base_hp/2 > ataca->actual_stats.base_hp){
            sprintf(log_acertou2, "%s errou o movimento e perdeu %.2f%% (%d/%d) de HP!\n\n", ataca->nome, (float)(ataca->actual_stats.base_hp/ataca->base_stats.base_hp), ataca->actual_stats.base_hp, ataca->base_stats.base_hp);
            ataca->actual_stats.base_hp = 0;
            strcat(log, log_acertou);
            strcat(log, log_acertou2);
            free(log_acertou2);
            free(log_acertou);
            return 0;
        }
        
        double porcentagem_perdida = ataca->actual_stats.base_hp / ataca->base_stats.base_hp/2;
        sprintf(log_acertou2, "%s errou o movimento e perdeu %.2f%% (%d/%d) de HP!\n\n", ataca->nome, porcentagem_perdida, ataca->base_stats.base_hp/2, ataca->base_stats.base_hp);
        ataca->actual_stats.base_hp -= ataca->base_stats.base_hp/2;
            strcat(log, log_acertou);
            strcat(log, log_acertou2);
            free(log_acertou2);
            free(log_acertou);
        return 0;
    }
    strcat(log, log_acertou);
    free(log_acertou);
    
    return ((100 * rand())/RAND_MAX < chance_final) ? 1 : 0;
}

void dmg_calc(Pokemon *ataca, Pokemon *recebe, int indice, char *log, int *dano_total){

    int dano = ataca->moves[indice].base_dmg;

    char *log_damage = malloc(sizeof(char) * 500);

    if (ataca->moves[indice].categoria == SPECIAL)
    {
        int special_attack = ataca->actual_stats.base_spa;
        int special_defense = recebe->actual_stats.base_spd;

        printf("\nBase Damage {%s} (ataca): %d", ataca->moves[indice].nome, dano);
        printf("\nSpecial Attack (ataca): %d", special_attack);
        printf("\nSpecial Defense (recebe): %d", special_defense);

        int modificador = modificadores(ataca, recebe, indice);
        int dano_bruto_dmg_calc = calc_dano_final(dano, special_attack, special_defense);
        
    
        *dano_total = (int) modificador * dano_bruto_dmg_calc;
    
        printf("Dano Bruto: %d\n", dano_bruto_dmg_calc);
        printf("Modificadores SPECIAL: %d\n", modificador);

    }
    else
    {
        int attack = ataca->actual_stats.base_atk;
        int defense = recebe->actual_stats.base_def;
        
        printf("\nBase Damage {%s} (ataca): %d", ataca->moves[indice].nome, dano);
        printf("\nAttack (ataca): %d", attack);
        printf("\nDefense (recebe): %d", defense);

        float mod = modificadores(ataca, recebe, indice);
        int dano_bruto_dmg_calc = calc_dano_final(dano, attack, defense);
        *dano_total = (int)  (mod * dano_bruto_dmg_calc);

        printf("Dano Bruto: %d\n", dano_bruto_dmg_calc);
        printf("Modificadores: %.1f\n", mod);
    }
    
    printf("\n\n\nDANO DADO:%d\n\n\n", *dano_total);
    printf("HP total: %d\n", recebe->base_stats.base_hp);
    printf("HP actual: %d", recebe->actual_stats.base_hp);
    system("sleep 2");
    // 

    double porcentagem_perdida = 0;

    if (*dano_total > recebe->actual_stats.base_hp)
        porcentagem_perdida = (double) (recebe->actual_stats.base_hp)/recebe->base_stats.base_hp;
    else
        porcentagem_perdida = (double) (*dano_total)/recebe->base_stats.base_hp;
        
    porcentagem_perdida *= 100;


    sprintf(log_damage, "\n\nThe opposing %s used %s!\n%s lost %.2f%% of its HP!\n\n", ataca->nome, ataca->moves[indice].nome, recebe->nome, porcentagem_perdida);

    printf("\nababo\n%s", log_damage);
    strcat(log, log_damage);
    free(log_damage);
    
    if ((*dano_total > recebe->actual_stats.base_hp))
        recebe->actual_stats.base_hp = 0;
    else
        recebe->actual_stats.base_hp -= *dano_total;

    //Debug
    system("sleep 2");

}

int calc_dano_final(int dano_bruto, int stat_ofensivo, int stat_defensivo){
    return 2 + (42 * (dano_bruto * stat_ofensivo/stat_defensivo)/50);
}

double modificadores(Pokemon *ataca, Pokemon *recebe, int indice){

    float multiplicador = 1;

    //STAB
    if (is_stab(ataca->types[0], ataca->types[1], ataca->moves[indice].type)){
        multiplicador *= 1.5;

        printf("\nÉ STAB\n");
    }

    //Critical hit
    if (will_cause_critical(ataca->moveCondition, ataca->moves[indice].funcao_move, ataca->tam_move_conditions)){
        multiplicador *= 1.5;
        printf("É CRITICAL HIT!\n");
    }
        

    //multiplicadores de bst
    if (ataca->moves[indice].categoria == SPECIAL)
    {

        printf("Multiplicador por stats: %.1f\n", calcular_nivel_multiplicador(ataca->multi.m_spa)*calcular_nivel_multiplicador(recebe->multi.m_spd));
        multiplicador *= calcular_nivel_multiplicador(ataca->multi.m_spa);
        multiplicador *= calcular_nivel_multiplicador(recebe->multi.m_spd);
    }
    else
    {
        printf("Multiplicador por stats: %.1f\n", calcular_nivel_multiplicador(ataca->multi.m_atk)*calcular_nivel_multiplicador(recebe->multi.m_def));
        multiplicador *= calcular_nivel_multiplicador(ataca->multi.m_atk);
        multiplicador *= calcular_nivel_multiplicador(recebe->multi.m_def);
    }
        
    printf("Multiplicador por super-efetividade: %.1f\n", calcula_super_efetividade(ataca->moves[indice].type, recebe));
    multiplicador *= calcula_super_efetividade(ataca->moves[indice].type, recebe);
    

    

    return multiplicador;
}

double calcula_super_efetividade(short int type_ataca, Pokemon *recebe){

    if (recebe->types[0] == recebe->types[1])
        return super_efetividade_monotype(type_ataca, recebe->types[0]);

    printf("O ADVERSÁRIO NÃO É MONOTYPE\n");
    return super_efetividade_monotype(type_ataca, recebe->types[0]) * super_efetividade_monotype(type_ataca, recebe->types[1]);

}

double super_efetividade_monotype(short int type_ataca, short int type_defende){
    

    char ataca_str[10], defende_str[10];
    sprintf(ataca_str, "%d", type_ataca);
    sprintf(defende_str, "%d", type_defende);
    
    FILE *arq_types = fopen("arquivos/types.txt", "r");
    if (!arq_types) return 1;
    
    char buffer[20000];
    int tipo_lido;

    // Verifica as propriedades defensivas do type_defende
    while (fgets(buffer, sizeof(buffer), arq_types)) {
        
        if (sscanf(buffer, "%d", &tipo_lido) == 1 && tipo_lido == type_defende) {
            // Encontrou o tipo defensor, verifica Weaknesses, Resistances, Immunities
            while (fgets(buffer, sizeof(buffer), arq_types)) {
                if (isdigit(buffer[0])) break; // Próximo tipo começou, não altera nada.
                
                if (strstr(buffer, "Weaknesses:")) {
                    if (strstr(buffer, ataca_str)) {
                        fclose(arq_types);
                        printf("\nDÁ SUPER EFETIVO\n");
                        return 2; // Super efetivo
                    }
                }
                else if (strstr(buffer, "Resistances:")) {
                    if (strstr(buffer, ataca_str)) {
                        printf("\nRESISTE AO MOVIMENTO\n");

                        fclose(arq_types);
                        return 0.5; // Resistência (0.5x damage)
                    }
                }
                else if (strstr(buffer, "Immunities:")) {
                    if (strstr(buffer, ataca_str)) {
                        fclose(arq_types);
                        printf("\nÉ IMUNE A ISSO\n");
                        return 0; // Imunidade
                    }
                    break; // Fim das propriedades defensivas
                }
            }
            break; // O tipo não foi encontrado
        }
    }
    
    fclose(arq_types);
    return 1;
}


int prioridade_por_velocidade(short int a, short int b){
    if (a == b) // speed tie gera uma condição randomica de quem vai primeiro
        return (rand() % 2) + 1;
    
    return (a > b) ? 1 : 2;
}

int consegue_atacar(Pokemon *ataca, char *log){
    int consegue = 1;
    char *log_tentou = malloc(sizeof(char) * 400);
    switch (ataca->statusCondition.condition)
    {
    case OK: break;
    case FREEZE:
        if (acerta(20)){
            sprintf(log_tentou, "%s se descongelou!\n", ataca->nome);
            ataca->statusCondition.condition = OK;
            ataca->statusCondition.turnos = 0;
        }
        else{
            sprintf(log_tentou, "%s está congelado!\n", ataca->nome);
            consegue = 0;
        }   
    case SLEEP:
        if (acerta(25)){
            sprintf(log_tentou, "%s despertou!\n", ataca->nome);
            ataca->statusCondition.condition = OK;
            ataca->statusCondition.turnos = 0;
        }
        else{
            sprintf(log_tentou, "%s está dormindo!\n", ataca->nome);
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
                strcat(log_tentou, ataca->nome);
                strcat(log_tentou, " está carregando o movimento!\n"); 
            }
                
            if (ataca->moveCondition[i].turnos < 1){
                retirar_move_condition(ataca, IN_CHARGE, i);
                return 1;
            }
                
            ataca->moveCondition[i].turnos--;
            return 0;
        case BLASTED:

            if (consegue == 1){
                strcat(log_tentou, ataca->nome);
                strcat(log_tentou, " está cansado e não consegue atacar!\n");
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
            strcat(log_tentou, ataca->nome);
            sprintf(log_tentou, "está se preparando para o contra-ataque (turnos restantes: %d)", ataca->moveCondition->turnos);
            if (ataca->moveCondition[i].turnos == 1)
                retirar_move_condition(ataca, BIDE, i);
            else ataca->moveCondition[i].turnos--;
        case FLINCH: 
            if (consegue == 1){            
            strcat(log_tentou, "O oponente ");
            strcat(log_tentou, ataca->nome);
            strcat(log_tentou, " hesitou e não pode se mover!\n");
            }
            retirar_move_condition(ataca, FLINCH, i);
        default:
            continue;
        }
    }

    printf("%s", log_tentou);
    strcat(log, log_tentou);
    free(log_tentou);
    return 1;
}

int acerta(double chance_base_acerto){
    return ((double) rand()/RAND_MAX < chance_base_acerto/100) ? 1 : 0;
}


// o único problema disso é que ele pode chegar no tamanho máximo de condições = menos de 10
void retirar_move_condition(Pokemon *poke, MCondition condition, int i){
    poke->moveCondition[i].condition = NONE;
}

int completar_moves(char *moves_buf, int tam){
    moves_buf[strcspn(moves_buf, "\n")] = '\0';
    int qtd = 1;

    for (int i = 0; moves_buf[i] != '\0'; i++)
        if (moves_buf[i] == '/')
            qtd++;

    if (qtd > 4)
        return 0; 

    while (qtd < 4){
        strcat(moves_buf, "/-");
        qtd++;
    }

    return 1;
}