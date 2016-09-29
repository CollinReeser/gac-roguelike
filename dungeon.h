#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>
#include <vector>

#include "entity.h"

class Dungeon
{
public:
    Dungeon();
    ~Dungeon();

    void print_dungeon();
    uint64_t get_width() const;
    uint64_t get_height() const;
    const Entity& entity_at_index(uint64_t x, uint64_t y) const;

private:
    const uint64_t DEFAULT_WIDTH = 80;
    const uint64_t DEFAULT_HEIGHT = 25;

    const uint64_t width;
    const uint64_t height;
    std::vector<std::vector<Entity>> dungeon;

    void generate_dungeon();
};

#endif
