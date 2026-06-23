#include "headers/erro.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* ======================================
   FUNÇÕES DE EXCEÇÃO (ORIGINAIS)
   ====================================== */

void nome_pokemon_exception(char *nome){
    printf("\n\nERRO: O pokémon '%s' não foi encontrado no banco de dados pokemons.txt\n", nome);
}

void arquivo_nao_encontrado_exception(char *filename){
    printf("\n\nERRO CRÍTICO: Arquivo \"%s\" não encontrado\n", filename);
    printf("Verifique se o arquivo existe no diretório 'arquivos/'\n\n");
}

void string_nao_encontrada_exception(char *nome, char *filename){
    printf("\n\n ERRO: A string \"%s\" não foi encontrada no arquivo: %s\n\n", nome, filename);
}

void move_nao_encontrado_exception(char *move, char *filename){
    printf("\n\n ERRO: O move \"%s\" não foi encontrado no arquivo: %s\n", move, filename);
    printf("Dica: Verifique a grafia e capitalização do move\n\n");
}

void tipos_exception(char *nome){
    printf("\n\nERRO: A função init_types() falhou ao inicializar os tipos para %s\n\n", nome);
}

int null_pointer_exception(){
    printf("ERRO FATAL: O dono desse PC não tem memória pra alocar");
    return 1;
}



int validar_tamanho_time(int tamanho) {

    if (tamanho > 0) return 1;
    
    printf("\n\nERRO: Tamanho de time inválido (%d)\n", tamanho);
    printf("O time deve ter no mínimo 1 Pokémon\n\n");
    return 0;
}

int validar_nome_player(char *nome) {
    if (nome == NULL) {
        printf("\n\n ERRO: Nome do jogador é nulo\n\n");
        return 0;
    }
    if (strlen(nome) == 0) {
        printf("\n\nERRO: Nome do jogador não pode ser vazio\n\n");
        return 0;
    }
    if (strlen(nome) > 50) {
        printf("\n\nERRO: Nome do jogador muito longo (max 50 caracteres)\n");
        printf("Você inseriu: %ld caracteres\n\n", strlen(nome));
        return 0;
    }
    return 1;
}

int validar_nome_pokemon(char *nome) {
    trocar_quebra_de_linha_por_terminador(nome);
    if (nome == NULL) {
        printf("\n\n ERRO: Nome do Pokémon é nulo\n\n");
        return 0;
    }
    if (strlen(nome) == 0) {
        printf("\n\n ERRO: Nome do Pokémon não pode ser vazio\n\n");
        return 0;
    }
    if (strlen(nome) > 20) {
        printf("\n\nERRO: Nome do Pokémon muito longo (máx 20 caracteres)\n");
        printf("Você inseriu: %ld caracteres\n\n", strlen(nome));
        return 0;
    }
    return 1;
}



int validar_pokemon_inicializado(Pokemon pokemon, char *nome) {
    if (pokemon.nome == NULL) {
        printf("\n\n ERRO ao inicializar Pokémon '%s'\n", nome);
        printf("A estrutura do Pokémon não foi preenchida corretamente\n\n");
        return 0;
    }
    
    if (pokemon.base_stats.base_hp == 0) {
        printf("\n\n ERRO: Pokémon '%s' tem HP base = 0\n", nome);
        printf("Verifique se o Pokémon existe no arquivo pokemons.txt\n\n");
        return 0;
    }
    
    if (pokemon.types[0] == 0 && pokemon.types[1] == 0) {
        printf("\n\nERRO: Pokémon '%s' não tem tipos atribuídos\n", nome);
        printf("Verifique se os tipos foram carregados corretamente\n\n");
        return 0;
    }
    
    int move_count = 0;
    for (int i = 0; i < 4; i++) {
        if (pokemon.moves[i].nome != NULL && strlen(pokemon.moves[i].nome) > 0) {
            move_count++;
        }
    }
    if (move_count == 0) {
        printf("\n\nERRO: Pokémon '%s' não tem nenhum move atribuído\n", nome);
        printf("Todo Pokémon deve ter pelo menos 1 move\n\n");
        return 0;
    }
    
    return 1;
}

int validar_time_inicializado(Pokemon *time, int tamanho, int player_num) {

    int pokemon_validos = 0;
    for (int i = 0; i < tamanho; i++) {
        if (time[i].nome == NULL) {
            printf("\n\nAVISO: Pokémon %d do jogador %d não foi inicializado\n", i + 1, player_num + 1);
        } else {
            pokemon_validos++;
        }
    }
    
    if (pokemon_validos == 0) {
        printf("\n\nERRO: Nenhum Pokémon válido no time do jogador %d\n\n", player_num + 1);
        return 0;
    }
    
    printf("Jogador %d: %d/%d Pokémons carregados com sucesso\n", player_num + 1, pokemon_validos, tamanho);
    return 1;
}

int validar_players_inicializados(Player player1, Player player2) {

    
    int valid = 1;
    
    if (!validar_nome_player(player1.nome)) {
        printf("Jogador 1 nome inválido\n");
        valid = 0;
    } else 
    {
        printf("Jogador 1: %s\n", player1.nome);
    }
    
    if (!validar_nome_player(player2.nome)) {
        printf("Jogador 2 nome inválido\n");
        valid = 0;
    } else 
    {
        printf("Jogador 2: %s\n", player2.nome);
    }
    
    if (!validar_time_inicializado(player1.time, player1.tam_time, 0)) {
        valid = 0;
    }
    
    if (!validar_time_inicializado(player2.time, player2.tam_time, 1)) {
        valid = 0;
    }
    
    printf("\n════════════════════════════════════════════════════════════\n\n");
    
    return valid;
}

void validar_malloc(void *ptr, char *contexto) {
    if (ptr == NULL) {
        int burrice_do_programador = null_pointer_exception();
        printf("Contexto: %s\n", contexto);
    }
}