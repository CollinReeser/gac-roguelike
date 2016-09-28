#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>

class Entity
{
public:
    Entity(char symbol, bool passable, int pos_x, int pos_y);
    ~Entity();

    char get_symbol() const;
    bool is_passable() const;
    int get_x() const;
    int get_y() const;
    void set_x(int x);
    void set_y(int y);

private:
    bool passable;
    bool construction;
    bool destructible;
    int pos_x;
    int pos_y;

    char symbol;
};

#endif
