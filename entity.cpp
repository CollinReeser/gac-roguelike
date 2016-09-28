#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entity.h"

Entity::Entity(char symbol, bool passable):
    symbol(symbol),
    passable(passable),
    construction(true),
    destructible(false)
{
}

char Entity::get_symbol() const
{
    return symbol;
}

Entity::~Entity()
{
}
