#include <string>

#include "weapon.h"

Weapon::Weapon(
    char symbol, std::string name, ALLEGRO_COLOR color,
    int pos_x, int pos_y,
    bool can_stack, uint64_t quantity,
    uint64_t throw_priority, uint64_t weight,
    uint64_t damage
):
    Item(
        symbol, name, color,
        pos_x, pos_y,
        can_stack, quantity,
        throw_priority, weight
    ),
    damage(damage)
{
}

Weapon* Weapon::copy() const {
    return new Weapon(
        get_symbol(), get_name(), get_color(),
        get_x(), get_y(),
        get_can_stack(), 1,
        get_throw_priority(), get_weight(),
        damage
    );
}

Weapon::~Weapon()
{
}
