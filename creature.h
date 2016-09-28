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
        char symbol,
        bool is_player, bool is_controllable,
        int pos_x, int pos_y,
        uint64_t speed
    );
    ~Creature();

    void set_position(int x, int y);

private:
    bool is_player;
    bool is_controllable;

    uint64_t speed;
};

#endif
