#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>

#include <stdint.h>
#include <vector>

class Entity
{
public:
    Entity(char symbol, bool passable, int64_t pos_x, int64_t pos_y);
    Entity(
        char symbol, ALLEGRO_COLOR color, bool passable,
        int64_t pos_x, int64_t pos_y
    );
    ~Entity();

    ALLEGRO_COLOR get_color() const;
    char get_symbol() const;
    bool is_passable() const;
    int64_t get_x() const;
    int64_t get_y() const;
    void set_position(int64_t x, int64_t y);
    void set_x(int64_t x);
    void set_y(int64_t y);

private:
    bool passable;
    int64_t pos_x;
    int64_t pos_y;

    char symbol;
    ALLEGRO_COLOR color;
};

#endif
