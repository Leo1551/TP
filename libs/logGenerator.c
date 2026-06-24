#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#include "headers/player.h"
#include "headers/logGenerator.h"
#include "headers/utilidades.h"
#include "headers/pokemon.h"

char* gerar_log_pre_batalha(Player *player1, Player *player2){

    char *novo_arquivo = gerar_arquivo_txt();

    FILE *arquivo = fopen(novo_arquivo, "w");
    if (!arquivo) {
        printf("\n\n\nErro ao abrir arquivo de log!\n\n\n");
        return;
    }

    // Exibe e armazena dados do Player 1
    fprintf(arquivo, "==================== PLAYER 1: %s ====================\n", player1->nome);
    printf("==================== PLAYER 1: %s ====================\n", player1->nome);
    
    for (unsigned int i = 0; i < player1->tam_time; i++)
        log_exibir_player(arquivo, player1, i);

    // Exibe e armazena dados do Player 2
    fprintf(arquivo, "\n\n==================== PLAYER 2: %s ====================\n", player2->nome);
    printf("\n\n==================== PLAYER 2: %s ====================\n", player2->nome);
    
    for (unsigned int i = 0; i < player2->tam_time; i++)
        log_exibir_player(arquivo, player2, i);

    fprintf(arquivo, "\n========================================================\n");
    printf("\n========================================================\n");
    
    fclose(arquivo);
    return novo_arquivo;

}

char* gerar_arquivo_txt(){
    char* str = malloc(70 * sizeof(char));
    char* id_batalha = gerar_id_batalha();
    
    strcpy(str, "log/battle_log_");
    strcat(str, id_batalha);
    strcat(str, ".txt");
    
    free(id_batalha);
    return str;
}

char* gerar_id_batalha(){
    // Criar diretório log se não existir
    struct stat st = {0};
    if (stat("log", &st) == -1) {
        mkdir("log", 0700);
    }
    
    // Seed do rand com timestamp para melhor aleatoriedade
    srand((unsigned int)time(NULL) + (unsigned int)rand());
    
    char* id = malloc(20 * sizeof(char));
    int id_gerado;
    int existe;
    
    do {
        existe = 0;
        id_gerado = 1000000 + (rand() % 9000000); // ID entre 1000000 e 9999999
        
        // Verifica se já existe arquivo com esse ID
        DIR *dir = opendir("log");
        if (dir != NULL) {
            struct dirent *entry;
            char nome_arquivo[100];
            
            while ((entry = readdir(dir)) != NULL) {
                sprintf(nome_arquivo, "battle_log_%d.txt", id_gerado);
                if (strcmp(entry->d_name, nome_arquivo) == 0) {
                    existe = 1; // ID já existe, precisa gerar outro
                    break;
                }
            }
            closedir(dir);
        }
    } while (existe);
    
    sprintf(id, "%d", id_gerado);
    return id;
}

void log_exibir_player(FILE *arquivo, Player *player, int i){

    Pokemon poke = player->time[i];
        
        show_info(poke);

        // Nome e tipos
        fprintf(arquivo, "\n[%u] %s\n", i + 1, poke.nome);
        
        // Stats base
        fprintf(arquivo, "HP: %d/%d\n", poke.actual_stats.base_hp, poke.base_stats.base_hp);
        
        fprintf(arquivo, "Atk: (%d) %d/%d\n", (int)poke.multi.m_atk, 
                (int)(poke.actual_stats.base_atk * calcular_nivel_multiplicador((int)poke.multi.m_atk)), 
                poke.base_stats.base_atk);
        
        fprintf(arquivo, "Def: (%d) %d/%d\n", (int)poke.multi.m_def, 
                (int)(poke.actual_stats.base_def * calcular_nivel_multiplicador((int)poke.multi.m_def)), 
                poke.base_stats.base_def);
        
        fprintf(arquivo, "SpA: (%d) %d/%d\n", (int)poke.multi.m_spa, 
                (int)(poke.actual_stats.base_spa * calcular_nivel_multiplicador((int)poke.multi.m_spa)), 
                poke.base_stats.base_spa);
        
        fprintf(arquivo, "SpD: (%d) %d/%d\n", (int)poke.multi.m_spd, 
                (int)(poke.actual_stats.base_spd * calcular_nivel_multiplicador((int)poke.multi.m_spd)), 
                poke.base_stats.base_spd);
        
        fprintf(arquivo, "Spe: (%d) %d/%d\n", (int)poke.multi.m_spe, 
                (int)(poke.actual_stats.base_spe * calcular_nivel_multiplicador((int)poke.multi.m_spe)), 
                poke.base_stats.base_spe);
        
        fprintf(arquivo, "Accuracy: %.2f | Evasion: %.2f\n", 
                calcular_nivel_multiplicador_accuracy((int)poke.multi.m_acc), 
                calcular_nivel_multiplicador_evasion((int)poke.multi.m_evasion));

        
        fprintf(arquivo, "Status: %s (Turnos: %d)\n", 
                show_status_condition(poke.statusCondition.condition), 
                poke.statusCondition.turnos);
        
        // Moves
        fprintf(arquivo, "--- Moves ---\n");
        for (int j = 0; j < 4; j++)
            if (poke.moves[j].nome != NULL) {
                fprintf(arquivo, "  %d. %s (Type: %d | Categoria: %d | DMG: %d | ACC: %.2f)\n",
                        j + 1, poke.moves[j].nome, poke.moves[j].type, poke.moves[j].categoria,
                        poke.moves[j].base_dmg, 
                        poke.moves[j].base_acc * calcular_nivel_multiplicador_accuracy((int)poke.multi.m_acc));
            }
            else fprintf(arquivo, " - \n");


}

void gerar_log_turno(char *acoes, char *farq){
    FILE *arq = fopen(farq, "a");

    fprintf(arq, acoes);
}

void gerar_log_vencedor_batalha(Player *player, char *farq, int num_player){

    FILE *arq = fopen(farq, "a");

    fprintf(arq, "===========================\n\nO VENCEDOR É O PLAYER %d!!\n\n", num_player);

    for (int i = 0; i < player->tam_time; i++)
        log_exibir_player(arq, player, i);
    

}



