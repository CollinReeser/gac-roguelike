#ifndef CREATURE_H
#define CREATURE_H

#include <allegro5/allegro.h>

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "entity.h"
#include "item.h"

class Creature : public Entity
{
public:
    Creature(
        char symbol, std::string name, ALLEGRO_COLOR color,
        bool player, bool controllable, bool friendly,
        uint64_t speed, uint64_t max_health,
        uint64_t strength, uint64_t dexterity,
        bool can_throw,
        std::unordered_map<char, Item*> items
    );
    ~Creature();

    bool is_player() const;
    bool is_controllable() const;
    bool is_friendly() const;

    bool is_turn(uint64_t clock_time) const;

    bool get_can_throw() const;
    uint64_t get_dexterity() const;
    int64_t get_health() const;
    uint64_t get_max_health() const;
    uint64_t get_strength() const;

    // Throw priority of 0 means you need to manually select that item to throw
    // as it is not considered a naturally-throwable item, like stones or
    // darts. So, when we ask "do you have a throwable thing?", assume by
    // default we mean a "naturall-throwable" thing
    bool has_throwable(uint64_t priority = 1) const;
    Item* consume_throwable();

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
    uint64_t dexterity;

    bool can_throw;

    std::unordered_map<char, Item*> items;
};

#endif
