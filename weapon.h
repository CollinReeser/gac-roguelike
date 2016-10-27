#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"

class Weapon : public Item
{
public:
    Weapon(
        char symbol, std::string name, ALLEGRO_COLOR color,
        int pos_x, int pos_y,
        bool can_stack, uint64_t quantity,
        uint64_t throw_priority, uint64_t weight,
        uint64_t damage
    );
    ~Weapon();

    Weapon* copy() const;

private:
    uint64_t damage;
};

#endif
