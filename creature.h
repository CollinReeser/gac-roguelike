#ifndef CREATURE_H
#define CREATURE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>

#include "entity.h"

class Creature : public Entity
{
public:
    Creature(
        bool is_player, bool is_controllable,
        uint64_t speed,
        char symbol
    );
    ~Creature();

private:
    bool is_player;
    bool is_controllable;

    uint64_t speed;
};

#endif
