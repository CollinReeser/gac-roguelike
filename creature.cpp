#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#include "creature.h"

Creature::Creature(
    char symbol, std::string name,
    bool player, bool controllable, bool friendly,
    int pos_x, int pos_y,
    uint64_t speed,
    uint64_t max_health
):
    Entity(symbol, false, pos_x, pos_y),
    name(name),
    player(player),
    controllable(controllable),
    friendly(friendly),
    speed(speed),
    max_health(max_health),
    health(max_health),
    strength(1)
{
}

bool Creature::is_player() const {
    return player;
}

bool Creature::is_controllable() const {
    return controllable;
}

bool Creature::is_friendly() const {
    return friendly;
}

bool Creature::is_turn(uint64_t clock_time) const {
    if (clock_time % speed == 0)
    {
        return true;
    }

    return false;
}

int64_t Creature::get_health() const {
    return health;
}

uint64_t Creature::get_max_health() const {
    return max_health;
}

std::string Creature::get_name() const {
    return std::string(name);
}

uint64_t Creature::get_strength() const {
    return strength;
}

void Creature::take_damage(uint64_t damage) {
    health -= damage;
}

Creature::~Creature()
{
}
