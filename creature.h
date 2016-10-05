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
        uint64_t speed, int64_t health
    );
    ~Creature();

    bool is_player();
    bool is_controllable();
    bool is_friendly();

    bool is_turn(uint64_t clock_time);

    int64_t get_health();
    uint64_t get_strength();

    void set_position(int x, int y);

    void take_damage(uint64_t damage);

private:
    bool player;
    bool controllable;
    bool friendly;

    uint64_t speed;
    int64_t health;

    uint64_t strength;
};

#endif
