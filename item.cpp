#include "item.h"

Item::Item(char symbol, int pos_x, int pos_y):
    symbol(symbol),
    pos_x(pos_x),
    pos_y(pos_y),
{
}

char Item::get_symbol() const
{
    return symbol;
}

int Item::get_x() const
{
    return pos_x;
}

int Item::get_y() const
{
    return pos_y;
}

void Item::set_x(int x)
{
    pos_x = x;
}

void Item::set_y(int y)
{
    pos_y = y;
}

Item::~Item()
{
}
