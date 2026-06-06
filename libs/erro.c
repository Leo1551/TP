#include "headers/erro.h"
#include <stdio.h>


void nome_pokemon_exception(char *nome){
    printf("\n\nO pokémon %s não foi encontrado no banco de dados pokemons.txt\nPerceba que o nome do Pokemon deve estar capitalizado\n", *nome);
}

void arquivo_nao_encontrado_exception(char *filename){
    printf("\n\nErro\n\nArquivo \"%s\" não encontrado\n\n", *filename);
}
void string_nao_encontrada_exception(char *nome, char *filename){
    printf("\n\nErro\n\n a String \"%s\" não foi encontrada no arquivo: %s\n\n", *nome, *filename);
}
void move_nao_encontrado_exception(char *move, char *filename){
    printf("\n\nErro\n\n O move \"%s\" não foi encontrado no arquivo: %s", *move, *filename);
}
void tipos_exception(char *nome){
    printf("\n\nErro\n\n A função init_types() falhou ao inicializar os types %s\n", *nome);
}