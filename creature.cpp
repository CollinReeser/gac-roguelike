#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "creature.h"

Creature::Creature(
    char symbol,
    bool is_player, bool is_controllable,
    int pos_x, int pos_y,
    uint64_t speed
):
    is_player(is_player),
    is_controllable(is_controllable),
    speed(speed),
    Entity(symbol, false, pos_x, pos_y)
{
}

Creature::~Creature()
{
}
