#ifndef ITEM_H
#define ITEM_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <string>

#include "entity.h"

class Item : public Entity
{
public:
    Item(
        char symbol, std::string name, ALLEGRO_COLOR color,
        int pos_x, int pos_y,
        bool can_stack, uint64_t quantity,
        uint64_t throw_priority, uint64_t weight
    );
    ~Item();

    uint64_t get_throw_priority() const;

    uint64_t get_quantity() const;
    void set_quantity(uint64_t quantity);

    bool get_can_stack() const;
    uint64_t get_weight() const;

    Item* copy(uint64_t quantity = 1) const;

private:
    bool can_stack;
    uint64_t quantity;
    uint64_t throw_priority;
    uint64_t weight;
};

#endif
