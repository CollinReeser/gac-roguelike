#ifndef CREATURE_H
#define CREATURE_H

#include <stdint.h>
#include <string>
#include <vector>

#include "entity.h"

class Creature : public Entity
{
public:
    Creature(
        char symbol, std::string name,
        bool player, bool controllable, bool friendly,
        int pos_x, int pos_y,
        uint64_t speed, uint64_t max_health
    );
    ~Creature();

    bool is_player() const;
    bool is_controllable() const;
    bool is_friendly() const;

    bool is_turn(uint64_t clock_time) const;

    int64_t get_health() const;
    uint64_t get_max_health() const;
    std::string get_name() const;
    uint64_t get_strength() const;

    void set_position(int x, int y);

    void take_damage(uint64_t damage);

private:
    std::string name;

    bool player;
    bool controllable;
    bool friendly;

    uint64_t speed;

    uint64_t max_health;
    int64_t health;

    uint64_t strength;
};

#endif
