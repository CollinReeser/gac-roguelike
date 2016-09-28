#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "creature.h"

Creature::Creature(
    bool is_player, bool is_controllable,
    uint64_t speed,
    char symbol
):
    is_player(is_player),
    is_controllable(is_controllable),
    speed(speed),
    Entity(symbol, false)
{
}

Creature::~Creature()
{
}
