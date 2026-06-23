#include <headers/moves.h>
#include <string.h>
#include <stdlib.h>


int is_stab(int tipo1, int tipo2, int tipo_move){
    return ((tipo1 == tipo_move) || (tipo2 == tipo_move));
}


void cause_move_effect(Pokemon *ataca, Pokemon *recebe, char *log, int indice_move, int dano_bruto){

    char *str = ataca->moves[indice_move].funcao_move;

    if (str == NULL || strlen(str) == 0) return;
    
    // Copia a string para strtok não modificar a original
    char *copy = malloc(strlen(str) + 1);
    strcpy(copy, str);
    
    // Tokeniza com delimitadores "->" e ","
    char *token = strtok(copy, "->,");
    
    while(token != NULL) {
        // Remove espaços em branco no início
        char *ptr = token;
        while(*ptr == ' ') ptr++;
        
        // Identifica e chama a função apropriada baseado no efeito
        if(strstr(ptr, "Burn")) {
            int chance_acerto = valores_em_parenteses(ptr, 1);
            cause_status_condition(recebe, BURN, chance_acerto);
        }
        else if(strstr(ptr, "Paralyze")) {
            int chance_acerto = valores_em_parenteses(ptr, 1);
            cause_status_condition(recebe, PARALYZE, chance_acerto);
        }
        else if(strstr(ptr, "Sleep")) {
            int chance_acerto = valores_em_parenteses(ptr, 1);
            cause_status_condition(recebe, SLEEP, chance_acerto);
        }
        else if(strstr(ptr, "Freeze")) {
            int chance_acerto = valores_em_parenteses(ptr, 1);
            cause_status_condition(recebe, FREEZE, chance_acerto);
        }
        else if(strstr(ptr, "BadlyPoison")) {
            apply_badly_poison(recebe);
        }
        else if(strstr(ptr, "Poison")) {
            int chance_acerto = valores_em_parenteses(ptr, 1);
            cause_status_condition(recebe, POISON, chance_acerto);
        }
        else if(strstr(ptr, "Confusion")) {
            cause_confusion(recebe);
        }
        else if(strstr(ptr, "Confuse")) {
            int chance_acerto = valores_em_parenteses(ptr, 1);
            will_cause_confusion(recebe, chance_acerto);
        }
        else if(strstr(ptr, "Flinch")) {
            will_cause_flinch(30);
        }
        else if(strstr(ptr, "MultiHit")) {
            cause_2_5_multihit(ataca, recebe, indice_move);
        }
        else if(strstr(ptr, "FixedDamage")) {
            int dano_bruto = valores_em_parenteses(ptr, 1);
            cause_fixed_dmg(recebe, 0);
        }
        else if(strstr(ptr, "Recoil")) {
        int chance_acerto = valores_em_parenteses(ptr, 1);

            cause_recoil_dmg(ataca, dano_bruto,chance_acerto);
        }
        else if(strstr(ptr, "Bind")) {
            cause_bind(recebe);
        }
        else if(strstr(ptr, "Charge")) {
            cause_charging_move(ataca);
        }
        else if(strstr(ptr, "Buff")) {
            cause_status_multiplier_change(*recebe, 1, 2);
        }
        else if(strstr(ptr, "Hit")) {
            int qtd_hits = valores_em_parenteses(ptr, 1);


            cause_hit_again(ataca, recebe, move, qtd_hits);
        }
        else if(strstr(ptr, "Crash")) {
            cause_crash(recebe, 50);
        }
        else if(strstr(ptr, "Disable")) {
           // cause_disable(recebe);
           printf("Não implementável no momento");
        }
        else if(strstr(ptr, "Taunt")) {
            cause_taunt(recebe);
        
        }
        else if(strstr(ptr, "Drain")) {
            cause_drain(recebe, dano_bruto, 50);
        }
        else if(strstr(ptr, "Seed")) {
            cause_leechseed(recebe, ataca);
        }
        else if(strstr(ptr, "Recover")) {
            recovery(recebe, 50);
        }
        else if(strstr(ptr, "Mimic")) {
            cause_mimic(recebe, ataca);
            //não implementável no momento
        }
        else if(strstr(ptr, "Consecutive")) {
            cause_consecutive_turns_move();
        }
        else if(strstr(ptr, "DreamEater") || strstr(ptr, "Dream_Eater")) {
            if (recebe->statusCondition.condition == SLEEP)
                cause_drain(ataca, dano_bruto, 50);
            
        }
        else if(strstr(ptr, "Psywave")) {
            psywave(recebe);
        }
        else if(strstr(ptr, "SuperFang")) {
            superfang(recebe);
        }
        else if(strstr(ptr, "Bide")) {
            bide();
            //não impleemntavel no momento
        }
        else if(strstr(ptr, "Dive")) {

            strcat(log, ataca->nome);
            strcat(log, " está bem fundo na água!\n");
            *(ataca->moveCondition) = (MoveCondition){DIVE, 1};
        }
        else if(strstr(ptr, "Dig")) {
             *(ataca->moveCondition) = (MoveCondition){DIG, 1};
        }
        else if(strstr(ptr, "cause_fly")) {
            cause_turn_wait(ataca, (MoveCondition){FLYING, 1});
            strcat(log, ataca->nome);
            strcat(log, " está bem alto nos céus!");
        }
        else if(strstr(ptr, "cause_dig")) {
            cause_turn_wait(ataca, (MoveCondition){DIG, 1});
        }
        else if(strstr(ptr, "OHKO")) {
            recebe->actual_stats.base_hp = 0;
        }
        else if(strstr(ptr, "Blasted")) {
            cause_recharge(ataca);
            // Recharge após Hyper Beam
        }
        else if(strstr(ptr, "Rest")) {
            recovery(recebe, 100);
        }
        
        token = strtok(NULL, "->");
    }
    
    free(copy);
}



int* valores_em_parenteses(char *str, int tam){
    return NULL;
}