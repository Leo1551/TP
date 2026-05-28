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
        
        printf("Insira o nome do seu %dº Pokémon: ", i + 1);
        // Insere nome do pokémon
        if (!fgets(nome_buf, sizeof(nome_buf), stdin)) nome_buf[0] = '\0';
        nome_buf[strcspn(nome_buf, "\n")] = '\0';
        capitalizarPalavras(nome_buf);

        printf("\nInsira os EVs (formato: HP/Atk/Def/Spa/Spd/Speed):\nEx: (252/0/0/4/0/252) note que 252 é o máximo por stat e o total é de 510 EVs para distribuir");
        // Insere EVs
        if (!fgets(evs_buf, sizeof(evs_buf), stdin)) evs_buf[0] = '\0';
        evs_buf[strcspn(evs_buf, "\n")] = '\0';

        printf("Insira os IVs (formato: HP/Atk/Def/Spa/Spd/Speed):\n Ex: (31/31/31/31/31/31) note que o máximo é 31 por IV");
        // Insere IVs
        if (!fgets(ivs_buf, sizeof(ivs_buf), stdin)) ivs_buf[0] = '\0';
        ivs_buf[strcspn(ivs_buf, "\n")] = '\0';

        printf("Insira os moves (separados por /):\n");
        //Insere moves
        if (!fgets(moves_buf, sizeof(moves_buf), stdin)) moves_buf[0] = '\0';
        moves_buf[strcspn(moves_buf, "\n")] = '\0';
        capitalizarPalavras(nome_buf);

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


