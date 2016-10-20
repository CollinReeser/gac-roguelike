#include <allegro5/allegro.h>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unordered_map>

#include "creature.h"

Creature::Creature(
    char symbol, std::string name, ALLEGRO_COLOR color,
    bool player, bool controllable, bool friendly,
    uint64_t speed, uint64_t max_health,
    uint64_t strength, uint64_t dexterity,
    bool can_throw,
    std::unordered_map<char, Item*> items
):
    Entity(symbol, name, false, -1, -1),
    name(name),
    player(player),
    controllable(controllable),
    friendly(friendly),
    speed(speed),
    max_health(max_health),
    health(max_health),
    strength(strength),
    dexterity(dexterity),
    can_throw(can_throw),
    items(items)
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

uint64_t Creature::get_strength() const {
    return strength;
}

uint64_t Creature::get_dexterity() const {
    return dexterity;
}

bool Creature::get_can_throw() const {
    return can_throw;
}

bool Creature::has_throwable(uint64_t priority) const {
    for (auto it = items.cbegin(); it != items.cend(); it++) {
        if (it->second->get_throw_priority() >= priority) {
            return true;
        }
    }

    return false;
}

// BUG: Can fail if there are no throwable items in the inventory
Item* Creature::consume_throwable() {
    uint64_t highest_priority = 0;
    Item* highest_priority_item = NULL;
    char item_index = '?';
    for (auto it = items.cbegin(); it != items.cend(); it++) {
        if (it->second->get_throw_priority() > highest_priority) {
            highest_priority = it->second->get_throw_priority();
            highest_priority_item = it->second;
            item_index = it->first;
        }
        else if (highest_priority_item == NULL) {
            highest_priority_item = it->second;
            item_index = it->first;
        }
    }

    if (highest_priority_item->get_quantity() == 1) {
        items.erase(items.find(item_index));
        return highest_priority_item;
    }
    else {
        highest_priority_item->set_quantity(
            highest_priority_item->get_quantity() - 1
        );
        return highest_priority_item->copy();
    }
}

void Creature::take_damage(uint64_t damage) {
    health -= damage;
}

Creature::~Creature()
{
}
