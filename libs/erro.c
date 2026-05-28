#include "headers/erro.h"
#include <stdio.h>


void nome_pokemon_exception(char *nome){
    printf("\n\nO pokémon %s não foi encontrado no banco de dados pokemons.txt\nPerceba que o nome do Pokemon deve estar capitalizado\n", nome);
}