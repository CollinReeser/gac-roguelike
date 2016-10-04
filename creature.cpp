#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "creature.h"

Creature::Creature(
    char symbol,
    bool player, bool controllable, bool friendly,
    int pos_x, int pos_y,
    uint64_t speed
):
    player(player),
    controllable(controllable),
    friendly(friendly),
    speed(speed),
    Entity(symbol, false, pos_x, pos_y)
{
}

bool Creature::is_player()
{
    return player;
}

bool Creature::is_controllable()
{
    return controllable;
}

Creature::~Creature()
{
}
