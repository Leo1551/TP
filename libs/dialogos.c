#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/utilidades.h"
#include "headers/player.h"
#include "headers/pokemon.h"

int* get_tamanho_times(){
    int *qtd = malloc(2 * sizeof(int));
    if (!qtd) return NULL;

    for (int i = 0; i < 2; i++){
        printf("Olá, seja bem-vind@, jogador@ %d\n\nEscolha quantos pokemon você vai usar: ", i + 1);
        if (scanf("%d", &qtd[i]) != 1) qtd[i] = 0;
    }
    return qtd;
}

Pokemon* criar_time(int tamanho){
    if (tamanho <= 0) return NULL;

    Pokemon *time = malloc(tamanho * sizeof(Pokemon));
    if (!time) return NULL;

    char nome_buf[128];
    char evs_buf[128];
    char ivs_buf[128];
    char moves_buf[256];

    limpar_buffer_de_teclado();
    for (int i = 0; i < tamanho; i++){
        
        do{
            printf("Insira o nome do seu %dº Pokémon: ", i + 1);
        } while (!fgets(nome_buf, sizeof(nome_buf), stdin));
        
        do{ //Insere EVs
            printf("\nInsira os EVs (formato: HP/Atk/Def/Spa/Spd/Speed):\nEx: (252/0/0/4/0/252) note que 252 é o máximo por stat e o total é de 510 EVs para distribuir");
        } while(!fgets(evs_buf, sizeof(evs_buf), stdin)); 
        
        do{ // Insere IVs
            printf("Insira os IVs (formato: HP/Atk/Def/Spa/Spd/Speed):\n Ex: (31/31/31/31/31/31) note que o máximo é 31 por IV");
        } while (!fgets(ivs_buf, sizeof(ivs_buf), stdin));
        
        do{
        printf("Insira os moves (separados por /):\n");
        //Insere moves
        } while(!fgets(moves_buf, sizeof(moves_buf), stdin));
        
        // aplica o terminador em tudo
        trocar_quebra_de_linha_por_terminador(nome_buf);
        trocar_quebra_de_linha_por_terminador(evs_buf);
        trocar_quebra_de_linha_por_terminador(ivs_buf);
        trocar_quebra_de_linha_por_terminador(moves_buf);

        //capitaliza nome e moves inseridos
        capitalizarPalavras(nome_buf);
        capitalizarPalavras(moves_buf);

        time[i] = init_pokemon(nome_buf, evs_buf, ivs_buf, moves_buf);
    }

    return time;
}




int* captar_stats(int max_por_stat, int max_total){
    int *stat = malloc(6 * sizeof(int));
    if (!stat) return NULL;

    const char *names[6] = {"HP", "Atk", "Def", "Spa", "Spd", "Speed"};
    int total = 0;

    while (1){
        total = 0;
        for (int i = 0; i < 6; i++){
            do{
                printf("\tInsira %s: ", names[i]);
                if (scanf("%d", &stat[i]) != 1) stat[i] = 0;
            } while (stat[i] < 0 || stat[i] > max_por_stat);
            total += stat[i];
        }
        if (total <= max_total) break;
        printf("Total de pontos (%d) excede o máximo permitido (%d). Tente novamente.\n", total, max_total);
    }
    return stat;
}


