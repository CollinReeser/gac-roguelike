#include <allegro5/allegro.h>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entity.h"

Entity::Entity(
    char symbol, std::string name,
    int64_t pos_x, int64_t pos_y,
    bool passable
):
    symbol(symbol),
    name(name),
    color(al_map_rgb(255, 255, 255)),
    pos_x(pos_x),
    pos_y(pos_y),
    passable(passable)
{
}

Entity::Entity(
    char symbol, std::string name, ALLEGRO_COLOR color,
    int64_t pos_x, int64_t pos_y,
    bool passable
):
    symbol(symbol),
    name(name),
    color(color),
    pos_x(pos_x),
    pos_y(pos_y),
    passable(passable)
{
}

ALLEGRO_COLOR Entity::get_color() const {
    return color;
}

std::string Entity::get_name() const {
    return std::string(name);
}

char Entity::get_symbol() const
{
    return symbol;
}

bool Entity::is_passable() const
{
    return passable;
}

int64_t Entity::get_x() const
{
    return pos_x;
}

int64_t Entity::get_y() const
{
    return pos_y;
}

void Entity::set_position(int64_t x, int64_t y) {
    pos_x = x;
    pos_y = y;
}

void Entity::set_x(int64_t x)
{
    this->pos_x = x;
}

void Entity::set_y(int64_t y)
{
    this->pos_y = y;
}

Entity::~Entity()
{
}
