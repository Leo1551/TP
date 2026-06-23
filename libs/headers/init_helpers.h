#include "pokemon.h"
#ifndef INITHELP_H
#define INITHELP_H

char* init_bst(char *nome);
Move* init_moves(char *moves_str);
int* init_types(char *nome);
int* init_monotype(const char *tipo);
int* init_dualtype(const char *tipo1, const char *tipo2);
int* init_stats(int max_por_stat, int max_total);

#endif