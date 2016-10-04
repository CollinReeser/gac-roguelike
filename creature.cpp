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
    Entity(symbol, false, pos_x, pos_y),
    player(player),
    controllable(controllable),
    friendly(friendly),
    speed(speed)
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

bool Creature::is_turn(uint64_t clock_time)
{
    if (clock_time % speed == 0)
    {
        return true;
    }

    return false;
}

Creature::~Creature()
{
}
