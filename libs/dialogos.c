#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/utilidades.h"
#include "headers/player.h"
#include "headers/pokemon.h"
#include "headers/init_helpers.h"
#include "headers/erro.h"

int* get_tamanho_times(){
    int *qtd = malloc(2 * sizeof(int));
    validar_malloc(qtd, "get_tamanho_times: alocação de qtd");

    for (int i = 0; i < 2; i++){
        printf("Olá, seja bem-vind@, jogador@ %d\n\nEscolha quantos pokemon você vai usar: ", i + 1);
        if (scanf("%d", &qtd[i]) != 1) qtd[i] = 0;
    }
    return qtd;
}
char* inserir_nome(){

    char *nome_jogador = malloc(sizeof(char) * 50);
    printf("Insira seu nome: ");
    scanf("%s", nome_jogador);

    return nome_jogador;

}


Pokemon* criar_time(int tamanho){
    if (tamanho <= 0) {
        printf("ERRO: Tamanho de time inválido\n");
        return NULL;
    }

    Pokemon *time = malloc(tamanho * sizeof(Pokemon));
    validar_malloc(time, "criar_time: alocação de time");

    char nome_buf[128];
    int* evs_buf = malloc(6 * sizeof(int));
    int* ivs_buf = malloc(6 * sizeof(int));
    char moves_buf[256];
    
    validar_malloc(evs_buf, "criar_time: alocação de evs");
    validar_malloc(ivs_buf, "criar_time: alocação de ivs");

    int pokemon_adicionados = 0;
    
    for (int i = 0; i < tamanho; i++){

            limpar_buffer_de_teclado();
            do{
                printf("\nInsira o nome do seu %dº Pokémon: ", pokemon_adicionados + 1);
            } while (!fgets(nome_buf, sizeof(nome_buf), stdin) && validar_nome_pokemon(nome_buf));
            
            
            // Insere EVs
            limpar_buffer_de_teclado();
            printf("\nInsira os EVs (formato: HP/Atk/Def/Spa/Spd/Speed):\nEx: (252/0/0/4/0/252) máx 252 por stat, total 510");
            evs_buf = init_stats(252, 510);
            
            // Insere IVs
            limpar_buffer_de_teclado();    
            printf("\nInsira os IVs (formato: HP/Atk/Def/Spa/Spd/Speed):\nEx: (31/31/31/31/31/31) máx 31 por IV");
            ivs_buf = init_stats(31, 31*6);
            
            limpar_buffer_de_teclado();
            do{
                printf("\nInsira os moves (separados por /):\n");
            } while(!fgets(moves_buf, sizeof(moves_buf), stdin));
            
            // Aplicar terminador em tudo
            trocar_quebra_de_linha_por_terminador(moves_buf);

            // Capitalizar nome e moves
            capitalizarPalavras(nome_buf);
            capitalizarPalavras(moves_buf);

        time[i] = init_pokemon(nome_buf, evs_buf, ivs_buf, moves_buf);    

        if (time[i].nome == NULL){
            erro_ao_inicializar_pokemon(nome_buf, evs_buf, ivs_buf, moves_buf);
            i--;
        }
        
    }

    return time;
}

void iniciar_batalha(Player player1, Player player2){
    
    char *log = malloc(50000 * sizeof(char));
    validar_malloc(log, "iniciar_batalha: alocação de log");

    system("clear");
    aviso_de_preguica_do_dev();

    int poke_player1 = player1.tam_time - 1;  // Começar pelo primeiro pokémon (índice 0)
    int poke_player2 = player2.tam_time - 1;
    int turno_atual = 1;

    // Gerar log pré-batalha
    char *arquivo_log = gerar_log_pre_batalha(player1, player2);


    // esse while serve para fazer toda a batalha em si, onde o loop só acaba quando um dos dois perderem (ou os dois, ao mesmo tempo)
    while (player1.tam_time != 0 || player2.tam_time != 0) 
        switch (quem_vence(&player1.time[poke_player1], &player2.time[poke_player2], &turno_atual, log, arquivo_log))
        {
            case 1:
                poke_player2--;
                break;
            case 2: 
                poke_player1--;
                break;
            default:
                poke_player1--;
                poke_player2--;
                break;
        }

    
    gerar_log_pos_batalha(player1, player2);
    free(log);
    
}

void gerar_log_pos_batalha(Player player1, Player player2){

    if (player1.tam_time == 0 && player2.tam_time == 0){
        gerar_log_vencedor_batalha(player1);
        gerar_log_vencedor_batalha(player2);
    }
    else if (player1.tam_time == 0)
        gerar_log_vencedor_batalha(player1);
    else
        gerar_log_vencedor_batalha(player2);

}

int quem_vence(Pokemon *poke1, Pokemon *poke2, int *turno_atual, char *log, char *nome_arquivo){


    while ((*poke1).actual_stats.base_hp >= 1 && (*poke2).actual_stats.base_hp >= 1)
    {
        char log_turno[10000];
        
        gerarTurno(poke1, poke2, turno_atual, log_turno);

        gerar_log_turno(log_turno, nome_arquivo);

        strcat(log, log_turno);
        *turno_atual++;
    }

    if ((*poke1).actual_stats.base_hp <= 0 && (*poke2).actual_stats.base_hp)
        return 3;
    if ((*poke1).actual_stats.base_hp <= 0)
        return 2;
    else 
        return 1;
    
}


void gerarTurno(Pokemon *poke1, Pokemon *poke2, int *turno, char *log){

    strcat(log, "\n\n_____________________");
    strcat(log, *turno);
    strcat(log, "_____________________\n\n");

    
    printf("%s", log);
    int acao_p1 = captar_indice_move(poke1);

    system("clear");

    printf("%s", log);
    int acao_p2 = captar_indice_move(poke2);
    
    system("clear");

    calcular_turno(poke1, acao_p1, poke2, acao_p2, log);

    printf("%s", log);

    system("clear");
}

int captar_indice_move(Pokemon *poke){

    show_info(*poke);

    printf("\n Qual move você deseja usar? (separe com espaco o nome do move. Ex: Thunder Punch)\nMove: ");

    char move_usado[25];
    int sim;
    int indice = 0;  
    int tentativas = 0;

    do{

        if(indice == -1) printf("Este move \'%s\' não foi encontrado! Tente novamente ou contate um técnico de TI\n", move_usado);

        
        tentativas++;

        sim = 0;
        while(sim != 1){
            printf("Move: ");
            scanf("%24s", move_usado);
              

            printf("\n\tDeseja usar: %s?\n(1 = sim / 0 = nao): ", move_usado);
            scanf("%d", &sim);
        }
        
        capitalizarPalavras(move_usado);
        indice = search_indice_move(poke->moves, move_usado);

    }while (indice == -1);

    return indice;
}

void calcular_turno(Pokemon *poke1, int move_poke1, Pokemon *poke2, int move_poke2, char *log){

    Pokemon *ataca_primeiro = NULL;
    Pokemon *outro = NULL;
    int prioridade = prioridade_por_velocidade(poke1->actual_stats.base_spe, poke2->actual_stats.base_spe);

    if (prioridade == 1) //define qual poke vai atacar primeiro
    {
        ataca_primeiro = poke1;
        outro = poke2;
    }
    else
    {
        ataca_primeiro = poke2;
        outro = poke1;
    }
    
    // AQUI É ONDE TEMOS APENAS O BRUTO, POKÉMON ATACA OUTRO
    move_calc(ataca_primeiro, outro, (prioridade == 1) ? move_poke1 : move_poke2, log);

    // se o poke inimigo sobreviveu, ele ataca agora
    if (outro->actual_stats.base_hp > 0)
        move_calc(outro, ataca_primeiro, (prioridade == 2) ? move_poke1 : move_poke2, log);
    else
    {
        strcat(log, outro->nome);
        strcat(log, " desmaiou!\n");
    }   
    // CALCULO PÓS TURNO DE QUEM ATACOU PRIMEIRO
    if (ataca_primeiro->actual_stats.base_hp > 0)
        strcat(log, gerar_condicoes_pos_turno(ataca_primeiro));

    if (ataca_primeiro->actual_stats.base_hp <= 0)
    {
        //Concatena a mensagem de morte
        strcat(log, ataca_primeiro->nome);
        strcat(log, " desmaiou!\n");
    }
    
    // CALCULO PÓS TURNO DE QUEM ATACOU PRIMEIRO
    if (outro->actual_stats.base_hp > 0)
        strcat(log, gerar_condicoes_pos_turno(outro));

    if (outro->actual_stats.base_hp <= 0)
    {
        //concatena a mensagem de morte
        strcat(log, outro->nome);
        strcat(log, " desmaiou!\n");
    }
    

}


void aviso_de_preguica_do_dev(){
    printf("\n\n\nO programador desse jogo é preguiçoso, então switch outs, trocas de pokémon e escolher qual será usado não é permitido até uma futura atualização do sistema (possivelmente qnd ele virar vagabundo de vez... Grato!)\n\n\n");
}

char* gerar_condicoes_pos_turno(Pokemon *poke, Pokemon *inimigo){
    
    char *string_saida[100]; 

    switch (poke->statusCondition.condition)
    {
    case BURN:
        apply_burn(poke);
        strcat(string_saida, poke->nome);
        strcat(string_saida, " perdeu 12.5 %% de danos de queimadura!\n");
        break;
    case POISON:
        apply_poison(poke);
        strcat(string_saida, poke->nome);
        strcat(string_saida, " tomou 12.5 %% danos de poison!\n");
    case BADLY_POISON:
        int damage = apply_badly_poison(poke);
        strcat(string_saida, poke->nome);
        strcat(string_saida, " tomou "); 
        sprintf(string_saida, "%.2f", damage/poke->base_stats.base_hp);
        strcat(string_saida,  " %% danos de queimadura!\n");
        poke->statusCondition.turnos++;
    default:
        return "";
    }

    int qtd_move_conditions =  sizeof(poke->moveCondition) / sizeof(MoveCondition);
    for (int i = 0; i < qtd_move_conditions; i++)
        switch (poke->moveCondition[i].condition)
        {
        case SEED:
            cause_leechseed(inimigo, poke);
            strcat(string_saida, inimigo->nome);
            strcat(string_saida, " teve a vida drenada por Leech Seed!\n");
            break;
        case BIND:
            cause_bind(poke);
            strcat(string_saida, poke->nome);
            strcat(string_saida, " se machucou por Bind!\n");

        default:
            break;
        }
        
    
    return string_saida;
    
}



