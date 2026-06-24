#include <headers/moves.h>
#include <headers/utilidades.h> //apenas para usar show_status_condition
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int is_stab(int tipo1, int tipo2, int tipo_move) {
    return ((tipo1 == tipo_move) || (tipo2 == tipo_move));
}


void cause_move_effect(Pokemon *ataca, Pokemon *recebe, char *log, int indice_move, int dano_bruto) {

    char *str = ataca->moves[indice_move].funcao_move;

    if (str == NULL || strlen(str) == 0) return;
    
    // Copia a string para strtok não modificar a original
    char *copy = malloc(strlen(str) + 1);
    strcpy(copy, str);
    
    char *token = strtok(copy, "->");
    
    while(token != NULL) {
        // Remove espaços em branco no início
        char *ptr = token;
        while(*ptr == ' ') ptr++;
        
        // Identifica e chama a função apropriada baseado no efeito
        if(strstr(ptr, "Burn") && recebe->statusCondition.condition == OK && recebe->types[0] != 1 && recebe->types[1] != 1) {
            int chance_acerto = valores_em_parenteses(ptr);
            cause_status_condition(recebe, BURN, chance_acerto, log);
        }
        else if(strstr(ptr, "Paralyze") && recebe->statusCondition.condition == OK && recebe->types[0] != 5 && recebe->types[1] != 5) {
            int chance_acerto = valores_em_parenteses(ptr);
            cause_status_condition(recebe, PARALYZE, chance_acerto, log);
        }
        else if(strstr(ptr, "Sleep") && recebe->statusCondition.condition == OK) {
            int chance_acerto = valores_em_parenteses(ptr);
            cause_status_condition(recebe, SLEEP, chance_acerto, log);
        }
        else if(strstr(ptr, "Freeze") && recebe->statusCondition.condition == OK && recebe->types[0] != 6 && recebe->types[1] != 6){
            int chance_acerto = valores_em_parenteses(ptr);
            cause_status_condition(recebe, FREEZE, chance_acerto, log);
        }
        else if(strstr(ptr, "BadlyPoison") && recebe->statusCondition.condition == OK && recebe->types[0] != 8 && recebe->types[1] != 8){
            cause_status_condition(recebe, BADLY_POISON, 100, log);
        }
        else if(strstr(ptr, "Poison") && recebe->statusCondition.condition == OK && recebe->types[0] != 8 && recebe->types[1] != 8){
            int chance_acerto = valores_em_parenteses(ptr);
            cause_status_condition(recebe, POISON, chance_acerto, log);
        }
        else if(strstr(ptr, "Confuse")){
            int chance_acerto = valores_em_parenteses(ptr);
            
            if (acerta(chance_acerto))
                cause_confusion(recebe);            
        }
        else if(strstr(ptr, "Flinch")){
            int chance_acerto = valores_em_parenteses(ptr);
            if (acerta(chance_acerto))
                add_move_condition(recebe, (MoveCondition){FLINCH, 1});
            
        }
        else if(strstr(ptr, "MultiHit")){
            cause_2_5_multihit(ataca, recebe, indice_move, log);
        }
        else if(strstr(ptr, "FixedDamage")){
            int dano_fixo = valores_em_parenteses(ptr);
            cause_fixed_dmg(recebe, dano_fixo);
        }
        else if(strstr(ptr, "Recoil")){
        int chance_acerto = valores_em_parenteses(ptr);

            cause_recoil_dmg(ataca, dano_bruto,chance_acerto);
        }
        else if(strstr(ptr, "Bind")){
            add_move_condition(recebe, (MoveCondition){BIND, 1});
        }
        else if(strstr(ptr, "Charge")){
           add_move_condition(ataca, (MoveCondition){IN_CHARGE, 1});
        }
        else if(strstr(ptr, "Buff")){
            int *v = valores_multiplos_em_parenteses(ptr, 3);
            if (acerta(v[2]))
                cause_status_multiplier_change(ataca, v[0], v[1]);
        }                                     // v[0] = tipo de buff, v[1] = quantidade
        else if(strstr(ptr, "Hit")){
            int qtd_hits = valores_em_parenteses(ptr);

            // -1 pois o primeiro hit já foi
            for (int i = 0; i < qtd_hits -1; i++)
                dmg_calc(ataca, recebe, indice_move, log, &dano_bruto);
            
        }
        else if(strstr(ptr, "Disable")){
           recebe->moves[indice_move].blocked_turns = 4;
        }
        else if(strstr(ptr, "Taunt")){
            cause_taunt(recebe);
        }
        else if(strstr(ptr, "Drain")){
            cause_drain(ataca, dano_bruto, 50);
        }
        else if(strstr(ptr, "Seed")){
            cause_leechseed(recebe, ataca);
        }
        else if(strstr(ptr, "Recover")){
            ataca->actual_stats.base_hp += ataca->base_stats.base_hp/2;
        }
        else if(strstr(ptr, "Mimic")){
            //cause_mimic(recebe, ataca);
            //não implementável no momento
        }
        else if(strstr(ptr, "Consecutive")){
            //cause_consecutive_turns_move();
            //não implementavel no momento
        }
        else if(strstr(ptr, "DreamEater") || strstr(ptr, "Dream_Eater")){
            if (recebe->statusCondition.condition == SLEEP)
                cause_drain(ataca, dano_bruto, 50);
            
        }
        else if(strstr(ptr, "Psywave")){
            psywave(recebe);
        }
        else if(strstr(ptr, "SuperFang")){
            superfang(recebe);
        }
        else if(strstr(ptr, "Bide")){
            // bide();
            //não impleemntavel no momento
        }
        else if(strstr(ptr, "Dive")){

            strcat(log, ataca->nome);
            strcat(log, " está bem fundo na água!\n");
            *(ataca->moveCondition) = (MoveCondition){DIVE, 1};
        }
        else if(strstr(ptr, "Dig")){
             *(ataca->moveCondition) = (MoveCondition){DIG, 1};
        }
        else if(strstr(ptr, "cause_fly")){
            cause_turn_wait(ataca, (MoveCondition){FLYING, 1});
            strcat(log, ataca->nome);
            strcat(log, " está bem alto nos céus!");
        }
        else if(strstr(ptr, "cause_dig")){
            cause_turn_wait(ataca, (MoveCondition){DIG, 1});
        }
        else if(strstr(ptr, "OHKO")){
            recebe->actual_stats.base_hp = 0;
        }
        else if(strstr(ptr, "Blasted")){
            cause_recharge(ataca);
            // Recharge após Hyper Beam
        }
        else if(strstr(ptr, "Rest")){
            recovery(recebe, 100);
        }
        
        token = strtok(NULL, "->");
    }
    
    free(copy);
}

void cause_status_condition(Pokemon *pokemon, SCondition tipo, float chance_base_acerto, char *log){
    
    if (acerta(chance_base_acerto)){
        sprintf(log, "O oponente %s recebeu %s!\n", pokemon->nome, show_status_condition(tipo));
        return;
    }
        
    
    pokemon->statusCondition.condition = tipo;
    pokemon->statusCondition.turnos = 0;
}

void cause_confusion(Pokemon *pokemon){
    pokemon->gambiarra_confusion = 1;
}
void cause_taunt(Pokemon *poke){
    for (int i = 0; i < 4; i++)
        if (poke->moves[i].categoria == STATUS)
            poke->moves[i].blocked_turns = 4;
}

void add_move_condition(Pokemon *pokemon, MoveCondition new_condition){
    
    pokemon->tam_move_conditions++;
    
    // Procura a primeira posição vazia
    int i = 0;
    while (pokemon->moveCondition[i].condition != NONE) {
        i++;
    }
    
    // Se não achou espaço vazio, realloca o array com um elemento a mais
    if (pokemon->moveCondition[i].condition == NONE && i > 0) {
        // Já encontrou posição vazia, só adiciona
        pokemon->moveCondition[i] = new_condition;
    } else {
        // Precisa realocar
        pokemon->moveCondition = realloc(pokemon->moveCondition, (i + 2) * sizeof(MoveCondition));
        pokemon->moveCondition[i] = new_condition;
        pokemon->moveCondition[i + 1].condition = NONE; // Marca próxima como vazia
        pokemon->moveCondition[i + 1].turnos = 0;
    }
}

void cause_status_multiplier_change(Pokemon *pokemon, int tipo, int nivel_modificador){

    switch (tipo)
    {
    case 1:
        pokemon->multi.m_atk += nivel_modificador;
        return;
    case 2:
        pokemon->multi.m_def += nivel_modificador;
        return;
    case 3:
        pokemon->multi.m_spa += nivel_modificador;
        return;
    case 4:
        pokemon->multi.m_spd += nivel_modificador;
        return;
    case 5:
        pokemon->multi.m_spe += nivel_modificador;
        return;
    case 6:
        pokemon->multi.m_acc += nivel_modificador;
        return;
    case 7:
        pokemon->multi.m_evasion += nivel_modificador;
        return;
    default: printf("\n\ndeu ruim aqui no moves/cause_status_multiplier_change\n\n");
        return;
    }
}


int* valores_multiplos_em_parenteses(char *str, int qtd){
    int *valores = malloc(sizeof(int) * qtd);

    // Encontra o primeiro '('
    char *inicio = strchr(str, '(');
    if (inicio == NULL) return valores;
    
    inicio++; // Move para após o '('
    
    char *copy = malloc(strlen(inicio) + 1);
    strcpy(copy, inicio);
    
    // Remove o ')'
    char *fim = strchr(copy, ')');
    if (fim != NULL) *fim = '\0';
    
    char *token = strtok(copy, ",");
    int index = 0;
    while (token != NULL && index < qtd)
    {
        valores[index++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    free(copy);
    return valores;
}

int valores_em_parenteses(char *str){
    char *inicio = strchr(str, '(');
    if (inicio == NULL) return 0;
    
    inicio++; // Move para após o '('
    char *fim = strchr(inicio, ')');
    
    if (fim == NULL) return 0;
    
    // Cria substring com o valor entre parênteses
    int tamanho = fim - inicio;
    char *valor_str = malloc(tamanho + 1);
    strncpy(valor_str, inicio, tamanho);
    valor_str[tamanho] = '\0';
    
    int resultado = atoi(valor_str);
    free(valor_str);
    
    return resultado;
}

// ==================== FUNÇÕES DE EFEITO DE MOVE ====================


void cause_2_5_multihit(Pokemon *ataca, Pokemon *recebe, int indice_move, char *log){
    // Gera quantidade de hits entre 2 e 5 com as probabilidades gen V
    int rand_val = rand() % 20;
    int qtd_hits;
    
    if (rand_val < 7) qtd_hits = 2;              // 35%
    else if (rand_val < 14) qtd_hits = 3;        // 35%
    else if (rand_val < 17) qtd_hits = 4;        // 15%
    else qtd_hits = 5;                            // 15%
    
    int *dano_bruto = malloc(sizeof(int));
    // Aplica cada hit
    for (int i = 0; i < qtd_hits; i++) {
        if (recebe->actual_stats.base_hp <= 0) break;
        dmg_calc(ataca,recebe, indice_move, log, dano_bruto);
    }

}

void cause_fixed_dmg(Pokemon *inimigo, int dano){
    
    if (dano > inimigo->actual_stats.base_hp)
        inimigo->actual_stats.base_hp = 0;
    else
        inimigo->actual_stats.base_hp -= dano;
    
}

void cause_recoil_dmg(Pokemon *pokemon, double dano_total_ao_adversario, double porcentagem_recoil){
    int dano_recoil = (int)(dano_total_ao_adversario * porcentagem_recoil / 100);
    
    if (dano_recoil > pokemon->actual_stats.base_hp) {
        pokemon->actual_stats.base_hp = 0;
    } else {
        pokemon->actual_stats.base_hp -= dano_recoil;
    }
}

void cause_turn_wait(Pokemon *pokemon, MoveCondition condition){
    add_move_condition(pokemon, condition);
}

void cause_drain(Pokemon *ataca, int dano_bruto, int porcentagem){
    int cura = (int)(dano_bruto * porcentagem / 100);
    
    ataca->actual_stats.base_hp += cura;
    if (ataca->actual_stats.base_hp > ataca->base_stats.base_hp) {
        ataca->actual_stats.base_hp = ataca->base_stats.base_hp;
    }
}

void cause_leechseed(Pokemon *recebe, Pokemon *ataca){
    add_move_condition(recebe, (MoveCondition){SEED, 1});
}

void cause_recharge(Pokemon *pokemon){
    add_move_condition(pokemon, (MoveCondition){BLASTED, 1});
}

void recovery(Pokemon *pokemon, int porcentagem){
    int cura = (int)(pokemon->base_stats.base_hp * porcentagem / 100);
    
    pokemon->actual_stats.base_hp += cura;
    if (pokemon->actual_stats.base_hp > pokemon->base_stats.base_hp) {
        pokemon->actual_stats.base_hp = pokemon->base_stats.base_hp;
    }
}

void psywave(Pokemon *recebe){
    // Psywave causa dano igual a (level/2) até (level + level/2) aleatório
    // Simplificado: causa dano fixo de 75
    int dano = 75;
    if (dano > recebe->actual_stats.base_hp) {
        dano = recebe->actual_stats.base_hp;
    }
    recebe->actual_stats.base_hp -= dano;
}

void superfang(Pokemon *recebe){
    // SuperFang causa dano igual a 50% do HP máximo
    int dano = recebe->actual_stats.base_hp / 2;
    recebe->actual_stats.base_hp -= dano;
}

void bide(Pokemon *pokemon){
    // Bide armazena dano e devolve em dobro no próximo turno
    add_move_condition(pokemon, (MoveCondition){BIDE, 2});
}

int will_cause_critical(MoveCondition *conditions, char *effect, int qtd){
    int nivel_crit = 0;

    for (int i = 0; i < qtd; i++)
        if (conditions[i].condition == CRIT)
        {
            nivel_crit = 1;
            break;
        }
        
    if (strstr(effect, "high_crit"))
        nivel_crit++;
    
    switch (nivel_crit)
    {
    case 0: return acerta(6.25);
    case 1: return acerta(12.5);
    case 2: return acerta(50);
    default:
        printf("\n\n\n\nERRO ESTRANHO EM will_cause_crit\n\n\n\n");
    }
    return 0;
}