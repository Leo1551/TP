#include "player.h"
#include <stdio.h>

#ifndef LOG_H
#define LOG_H

/*

    Criará um arquivo único battle_log_<idbatalha>.txt
        - Guardará todos os pokes do p1, p2
*/
char* gerar_log_pre_batalha(Player *player1, Player *player2);


/*
    Gera um ID único para a batalha lendo arquivos existentes no diretório /log
    Retorna uma string com o ID aleatório que não conflita com outros
*/
char* gerar_id_batalha();

/*
    Gera o caminho do arquivo de log (battle_log_<id>.txt) e cria diretório log se necessário
*/
char* gerar_arquivo_txt();

/*
    Receberá uma string formatada com o que será exibido no terminal
    e guardará no arquivo battle-log-<idbatalha>.txt
*/
void gerar_log_turno(char *acoes, char *farq);



/*
    Escreve no arquivo battle-log-<idbatalha.txt>.txt
        - Nome do vencedor 
        - Todos os Pokémon
*/
void gerar_log_vencedor_batalha(Player *player, char *arq, int num_player);
/*

*/
void log_exibir_player(FILE *arquivo, Player *player, int i);


#endif