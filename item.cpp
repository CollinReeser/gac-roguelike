#include <string>

#include "item.h"

Item::Item(
    char symbol, std::string name, ALLEGRO_COLOR color,
    int pos_x, int pos_y,
    bool can_stack, uint64_t quantity,
    uint64_t throw_priority, uint64_t weight
):
    Entity(symbol, name, color, true, pos_x, pos_y),
    can_stack(can_stack),
    quantity(quantity),
    throw_priority(throw_priority),
    weight(weight)
{
}

uint64_t Item::get_throw_priority() const {
    return throw_priority;
}

uint64_t Item::get_quantity() const {
    return quantity;
}

void Item::set_quantity(uint64_t quantity) {
    this->quantity = quantity;
}

bool Item::get_can_stack() const {
    return can_stack;
}

uint64_t Item::get_weight() const {
    return weight;
}

Item* Item::copy(uint64_t quantity) const {
    return new Item(
        get_symbol(), get_name(), get_color(),
        get_x(), get_y(),
        can_stack, quantity,
        throw_priority, weight
    );
}

Item::~Item()
{
}
