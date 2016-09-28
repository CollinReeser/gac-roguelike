#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>

class Entity
{
public:
    Entity(char symbol, bool passable);
    ~Entity();

    char get_symbol() const;

private:
    bool passable;
    bool construction;
    bool destructible;

    char symbol;
};

#endif
