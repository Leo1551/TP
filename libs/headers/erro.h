#ifndef ERRO_H
#define ERRO_H

void nome_pokemon_exception(char *nome);
void arquivo_nao_encontrado_exception(char *filename);
void string_nao_encontrada_exception(char *nome, char *filename);
void move_nao_encontrado_exception(char *move, char *filename);
void tipos_exception(char *nome);
#endif