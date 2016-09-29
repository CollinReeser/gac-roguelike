#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entity.h"

Entity::Entity(char symbol, bool passable, int pos_x, int pos_y):
    symbol(symbol),
    passable(passable),
    pos_x(pos_x),
    pos_y(pos_y),
    construction(true),
    destructible(false)
{
}

char Entity::get_symbol() const
{
    return symbol;
}

bool Entity::is_passable() const
{
    return passable;
}

int Entity::get_x() const
{
    return pos_x;
}

int Entity::get_y() const
{
    return pos_y;
}

void Entity::set_x(int x)
{
    this->pos_x = x;
}

void Entity::set_y(int y)
{
    this->pos_y = y;
}

Entity::~Entity()
{
}
