#ifndef PLAYER_H
#define PLAYER_H

#include "pokemon.h"

typedef struct Player{
    char *nome;
    Pokemon *time;
    unsigned int tam_time;
}Player;

#endif
