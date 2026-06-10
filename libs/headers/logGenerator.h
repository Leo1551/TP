#include "player.h"


/*

    Criará um arquivo único battle_log_<idbatalha>.txt
        - Guardará todos os pokes do p1, p2
*/
void gerar_log_pre_batalha(Player player1, Player player2);


/*
    O critério será uma leitura do diretório /log, dependendo de quantos arquivos tiverem lá dentro
    e um número rand(1000), será gerado um id

*/
int gerar_id_batalha();

/*
    Receberá uma string formatada com o que será exibido no terminal
    e guardará no arquivo

*/
void gerar_log_turno(char *acoes, int turno);



/*
    Escreve no arquivo battle-log-<idbatalha.txt>.txt
        - Nome do vencedor 
        - Todos os Pokémon
*/
void gerar_log_vencedor_batalha(Player player1);