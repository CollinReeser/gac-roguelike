#ifndef CREATURE_H
#define CREATURE_H

#include <stdint.h>
#include <vector>

#include "entity.h"

class Creature : public Entity
{
public:
    Creature(
        char symbol,
        bool player, bool controllable, bool friendly,
        int pos_x, int pos_y,
        uint64_t speed
    );
    ~Creature();

    bool is_player();
    bool is_controllable();

    void set_position(int x, int y);

private:
    bool player;
    bool controllable;
    bool friendly;

    uint64_t speed;
};

#endif
